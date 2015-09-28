#include <curl/curl.h>
#include <string>
#include <time.h>
#include <sys/stat.h>
#include <io.h>
#include <vector>
#include <direct.h>
#include "md5.h"
#include "Downloader.h"
#include "AppHelper_Win32.h"

#ifdef WIN32
#include <windows.h> // ��VC��ʹ�ô���ͷ�ļ�
#else
#include <unistd.h>  // ��gcc�������У�ʹ�õ�ͷ�ļ���gcc�汾�Ĳ�ͬ����ͬ
#endif

//����xml
#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_print.hpp"
#include "rapidxml/rapidxml_utils.hpp"



std::vector<std::string> DownLoadHelper::split( std::string str, std::string separator )
{  
	std::vector<std::string>result;  
	int cutAt;  
	while( (cutAt = str.find_first_of(separator)) != str.npos )  
	{  
		if(cutAt > 0)  
		{  
			result.push_back(str.substr(0, cutAt));  
		}  
		str = str.substr(cutAt + 1);  
	}  
	if(str.length() > 0)  
	{  
		result.push_back(str);  
	}  
	return result;  
}  

std::string& DownLoadHelper::trim( std::string &s )
{
	if (s.empty())
	{
		return s;
	}

	s.erase(0,s.find_first_not_of(" "));
	s.erase(s.find_last_not_of(" ") + 1);
	return s;
}

std::string DownLoadHelper::FileMd5( std::string filename )
{
	FILE * fp = fopen(filename.c_str(),"rb");
	if (fp == NULL)
		return "";

	int nLen = 0;
	if ( nLen )
	{
		fseek(fp, 0, SEEK_END); 
		nLen = ftell(fp);
		fseek(fp, 0, SEEK_SET);
	}


	md5_state_t state;
	md5_byte_t digest[16];
	char hex_output[16*2 + 1];
	size_t file_size = 0;
	const int buf_size = 1024;

	char buf[1024] = {0};

	md5_init(&state);
	do
	{
		size_t readsize = fread(buf, 1, buf_size, fp);
		file_size += readsize;
		md5_append(&state, (const md5_byte_t *)buf, readsize);

	} 
	while( !feof(fp) && !ferror(fp) );

	md5_finish(&state, digest);

	for (int di = 0; di < 16; ++di)
		sprintf(hex_output + di * 2, "%02x", digest[di]);

	hex_output[16*2] = 0;

	return std::string(hex_output);
}

bool DownLoadHelper::MakeDir( const char* path )
{
#if defined(_WIN32)
	return ::_mkdir(path) == 0;
#else
	return ::mkdir(path, S_IRWXU | S_IRWXG | S_IRWXO) == 0;
#endif
}

void DownLoadHelper::CreateDir( std::string filepath )
{
	std::vector<std::string> fileallpth = split(filepath,"/");
	std::string filepathext = "";
	for (int i = 0; i < fileallpth.size(); ++i)
	{
		std::string filefloder = trim(fileallpth.at(i));
		if (!filefloder.empty())
		{
			if (i == 0)
			{
				filepathext += filefloder;
			}
			else
			{
				filepathext +="/";
				filepathext += filefloder;
			}

			if (DownLoadHelper::Access(filepathext.c_str(),0) != 0)
			{
				//������
				MakeDir(filepathext.c_str());
			}
		}
		else
			break;
	}
}

unsigned int DownLoadHelper::xnGetTickCount()
{

#ifdef WIN32
	return (unsigned int)GetTickCount();
#else
	//gettickcount
	struct timespec ts;  

	clock_gettime(CLOCK_MONOTONIC, &ts);  

	return (ts.tv_sec * 1000 + ts.tv_nsec / 1000000); 
#endif
}

unsigned int DownLoadHelper::GetLocalFileSize( std::string filepath )
{
	if (filepath.empty())
		return 0;

	struct _stat fileinfo;
	int result =_stat( filepath.c_str(), &fileinfo ); 
	if (result == 0)
	{
		return fileinfo.st_size;
	}
	else
		return 0;
}

int DownLoadHelper::Access( const char * szFile,int mode )
{
	return _access(szFile,mode);
}

int DownLoadHelper::Remove( const char * szFile )
{
	return remove(szFile);
}

int DownLoadHelper::Rename( const char *oldname,const char *newname )
{
	return rename(oldname,newname);
}

void DownLoadHelper::HttpSleep( int nMillsec )
{
#ifdef WIN32
	Sleep(1);
#else
	sleep(1);
#endif
}

DownLoader::DownLoader()
{
	CURLcode returncode = curl_global_init(CURL_GLOBAL_ALL);
	if (returncode != CURLE_OK)
	{
		//logerror
	}

	m_bResDownloadFininsh = false;
	m_bExeDownloadFininsh = false;
	m_nAppVersion = 0;
	m_bInstall  = false;
	m_nAppVersion = AnalyzeVersion("Version_old.xml",m_VersionAppName);

	pthread_mutex_init(&m_mutex,NULL);
	pthread_cond_init(&m_downloadappcond,NULL);

	AppHelperWin32::GetInstancePtr();
}

DownLoader::~DownLoader()
{
	//�ȴ�����
	pthread_join(m_downloadthread,NULL);
	pthread_join(m_downloadappthread,NULL);
	pthread_cond_destroy(&m_downloadappcond);
	pthread_mutex_destroy(&m_mutex);

	curl_global_cleanup();	//�������
}

bool DownLoader::SetBastUrl( std::string baseUrl )
{
	m_baseUrl = baseUrl;

	return true;
}

bool DownLoader::DownFileList()
{
	//���ļ�����ɾ��
	if (DownLoadHelper::Access("files.txt",0) == 0)
	{	
		DownLoadHelper::Remove("files.txt");
	}
	std::string listUrl = m_baseUrl + "resources/files.txt";

	DownloadFile("files.txt",listUrl,false);
	AnalyzeFileList();
	//PrintDownList();
	StartDownloadThread();

	//���������߳�
	return true;
}

bool DownLoader::DownloadFile( std::string szFileName,std::string fileUrlList,bool bAdd)
{
	CURL * easy_handle = curl_easy_init();
	if (easy_handle  == NULL)
	{
		printf("init failed!");
		return false;
	}

	char szErrorBuffer[512] = {0};
	FILE * fp = NULL;
	
	if (bAdd)
	{
		fp = fopen(szFileName.c_str(),"ab+");
	}
	else
	{
		fp = fopen(szFileName.c_str(),"wb");
	}
	

	int localfilesize = 0;
	if (bAdd)
	{
		localfilesize = DownLoadHelper::GetLocalFileSize(szFileName);
	}
	else
	{
		localfilesize  = 0;
	}
	
	int filesize = 0;

	do 
	{
		//����url
		CC_BREAK_IF(curl_easy_setopt(easy_handle,CURLOPT_URL,fileUrlList.c_str())!= CURLE_OK);
		//����û�����֤
		//CC_BREAK_IF(curl_easy_setopt(easy_handle,CURLOPT_USERPWD,"zh423328:423328") != CURLE_OK);

		//�Ƿ����httpͷ
		//	CC_BREAK_IF(curl_easy_setopt(easy_handle,CURLOPT_HEADER,0) != CURLE_OK);

		//����http ͷ��������
		//��ȡ�ļ���С
		CC_BREAK_IF(curl_easy_setopt(easy_handle, CURLOPT_HEADERFUNCTION, GetContentLengthFunc) != CURLE_OK );

		CC_BREAK_IF(curl_easy_setopt(easy_handle, CURLOPT_HEADERDATA, &filesize) != CURLE_OK);

		//no singanl,���߳�
		CC_BREAK_IF(curl_easy_setopt(easy_handle,CURLOPT_NOSIGNAL,1L) != CURLE_OK);

		//ʹ��https��֤,�����Ȳ�����֤֤���HOST
		CC_BREAK_IF(curl_easy_setopt(easy_handle,CURLOPT_SSL_VERIFYPEER,0) != CURLE_OK);
		CC_BREAK_IF(curl_easy_setopt(easy_handle,CURLOPT_SSL_VERIFYHOST,0) != CURLE_OK);

		//�ϵ�ָ��ϴ�
		//�ӵ�ǰ�ļ���С����д,��CURLOPT_RESUME_FROM ������large
		CC_BREAK_IF(curl_easy_setopt(easy_handle,CURLOPT_RESUME_FROM,localfilesize) != CURLE_OK);
		//�����ļ���ȡ
		CC_BREAK_IF(curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, writefunc)!= CURLE_OK);  
		CC_BREAK_IF(curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, fp)!= CURLE_OK);		//���ö�ȡ

		//���������ļ���ȡ����
		CC_BREAK_IF(curl_easy_setopt(easy_handle,CURLOPT_NOPROGRESS,0) != CURLE_OK);

		//���ý��Ⱥ���
		CC_BREAK_IF(curl_easy_setopt(easy_handle,CURLOPT_PROGRESSFUNCTION,progress_callback) != CURLE_OK);

		//����client flag
		CC_BREAK_IF(curl_easy_setopt(easy_handle,CURLOPT_PROGRESSDATA,szFileName) != CURLE_OK);

		//���ô�ӡ
		//CC_BREAK_IF(curl_easy_setopt(easy_handle, CURLOPT_VERBOSE, 1L) != CURLE_OK);		//��ӡ

	} while (0);

	//ִ��
	CURLcode code = curl_easy_perform(easy_handle);		//�������
	if (code != CURLE_OK)
	{
		printf("error:%s\n",curl_easy_strerror(code));
		curl_easy_cleanup(easy_handle);
		return false;
	}

	long resultCode = 0;
	curl_easy_getinfo(easy_handle, CURLINFO_RESPONSE_CODE, &resultCode);
	fclose(fp);//�ر�ˢ��
	curl_easy_cleanup(easy_handle);
	return true;
}

bool DownLoader::AnalyzeFileList()
{
	m_filelist.clear();
	m_vAlreadyFileList.clear();

	//��ȡ�ļ�
	FILE * fp = fopen("files.txt","rb");
	if (fp == NULL)
		return false;

	char szLine[1024] = {0};

	//��ȡ����
	std::vector<std::string> files;		//�ļ��б�
	//�Ƿ��ǽ�β
	while(!feof(fp))
	{
		memset(szLine,0,1024);
		fgets(szLine,1024,fp);
		std::string tmp = szLine;
		DownLoadHelper::trim(tmp);
		if (!tmp.empty())
	 		files.push_back(szLine);
	}

	m_vAlreadyFileList.push_back(std::make_pair("tmp","tmp"));
	for (int i = 0; i < files.size(); ++i)
	{
		std::string file = files[i];

		std::vector<std::string> keyValue = DownLoadHelper::split(file,"|");
		if (keyValue.size()>= 2)
		{
			std::string filename = keyValue[0];
			std::string md5Value = keyValue[1];
			filename = DownLoadHelper::trim(filename);
			md5Value = DownLoadHelper::trim(md5Value);

			//�ж��ļ��Ƿ���Ҫ����
			bool bUpdate = false;			
			int nPos =  filename.find_last_of("/");
			if (nPos != std::string::npos)
			{
				//���ǽ�β�����߲�����
				std::string path = filename.substr(0,nPos);

				if (!path.empty())
				{
					//����������ļ���ʱ�����ļ�
					if (DownLoadHelper::Access(path.c_str(),0) != 0)	//access��ƽ̨
					{
						//�����ļ�
						//mkdir(path.c_str());
						DownLoadHelper::CreateDir(path);
					}

					//Ȼ���ж��Ƿ���Ҫ����
				}
			}

			//�ж�md5 
			if (DownLoadHelper::Access(filename.c_str(),0) == 0)
			{
				//�����ж�md5
				std::string filemd5 = DownLoadHelper::FileMd5(filename);
				bool bUpdate = (filemd5 == md5Value);	

				if (bUpdate)
					DownLoadHelper::Remove(filename.c_str());
			}
			else
			{
				bUpdate = true;
			}

			if (bUpdate)
			{
				//����
				std::string  listUrl = m_baseUrl +"resources/"+ filename;
				m_vAlreadyFileList.push_back(std::make_pair(filename,listUrl));
			}
		}
	}

	return true;
}

bool DownLoader::StartDownloadThread()
{
	int nRet = 0;
	nRet = pthread_create(&m_downloadappthread,NULL,DownloadApp,this);
	nRet = pthread_create(&m_downloadthread,NULL,Worker,this);
	if (nRet == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//download

//static 
//write data to upload  
size_t DownLoader::writefunc(void *ptr, size_t size, size_t nmemb, void *stream)  
{  
	return fwrite(ptr, size, nmemb, (FILE*)stream);  
}  

//�����߳�
void * DownLoader::Worker( void *param )
{
	DownLoader *loader = (DownLoader*)param;
	if (loader)
	{
		while(loader->m_vAlreadyFileList.size())
		{
			std::pair<std::string,std::string> FileInfo = loader->m_vAlreadyFileList.at(0);
			if (FileInfo.first == "tmp")
			{

			}
			else
			{
				loader->DownloadFile(FileInfo.first,FileInfo.second,false);
			}
			
			loader->m_vAlreadyFileList.erase(loader->m_vAlreadyFileList.begin());

			//printf("download :%s\n",FileInfo.first.c_str());
			if (loader->m_vAlreadyFileList.size() == 0)
			{
				pthread_mutex_lock(&loader->m_mutex);
				loader->m_bResDownloadFininsh = true;
				//����ֻ�����Զ���,��ʵһ���߳̾͹���
				pthread_cond_signal(&loader->m_downloadappcond);		
				pthread_mutex_unlock(&loader->m_mutex);
				break;
			}

			DownLoadHelper::HttpSleep(1);
		}
	}

	return NULL;
}

//////////////////////////////////////////////////////////////////////////
//����1:�Զ�������
//����2:�ܵ���Ҫ���ض����ļ�
//����3:�Ѿ����˶���
//����4:��Ҫ�ϴ��ļ��Ĵ�С
//����5:�Ѿ��ϴ��˵Ĵ�С
//////////////////////////////////////////////////////////////////////////
int DownLoader::progress_callback(void *clientp,   double dltotal,   double dlnow,   double ultotal,   double ulnow)
{
	double percent = 0;

	if (dltotal)
		percent = dlnow/dltotal;

	//unsigned int now = xnGetTickCount();
	//double speed = 0;

	//int dela = now - g_nStart;

	//char * szFileName = (char*)clientp;
	//if (szFileName)
	//{

	//	if (dela)
	//	{
	//		speed = dlnow*1.0/dela*1000;
	//	}

	//	if (speed > 1024*1024)
	//	{
	//		speed = speed/1024/1024;
	//		printf("%s:%.2f speed:%.2fMb/s\n",szFileName,percent*100,speed);
	//	}
	//	else if (speed > 1024)
	//	{
	//		speed = speed/1024;
	//		printf("%s:%.2f speed:%.2fKb/s\n",szFileName,percent*100,speed);
	//	}

	//}

	return 0;
}

//version
int DownLoader::AnalyzeVersion(char * szVersionFileName,std::string &fileApp)
{	
	std::string strFileName = szVersionFileName;
	if (DownLoadHelper::Access(szVersionFileName,0) == 0)
	{
	}
	else
	{
		//ȥ��old
		int nPos = strFileName.rfind("_old");
		if (nPos != std::string::npos)
		{
			strFileName.replace(nPos,4,"");
		}
		
		if (DownLoadHelper::Access(strFileName.c_str(),0) != 0)
		{
			return 0;
		}
	}

	if (!strFileName.empty())
	{
		//�Ƿ����
		//����
		try
		{
			rapidxml::file<> fdoc(strFileName.c_str());

			rapidxml::xml_document<> doc;
			doc.parse<0>(fdoc.data());				//����

			rapidxml::xml_node<> *pRoot = doc.first_node();			//�Ƿ��нڵ�
			if (pRoot == NULL)
			{
				return 0;
			}
			else
			{
				int nVersion = 0;
				rapidxml::xml_node<> *pNode = pRoot->first_node("app_version");
				if (pNode)
				{
					nVersion = atoi(pNode->value());
				}

				pNode = pRoot->first_node("app_name");
				if (pNode)
				{
					fileApp = pNode->value();
				}
				return nVersion;
			}
		}
		catch (std::exception e)
		{
			printf("parse error:%s\n",e.what());
		}
		catch(...)
		{
			printf("parse error");
		}
	}

	return 0;
}

void * DownLoader::DownloadApp( void *pParam )
{
	DownLoader *pLoader = (DownLoader*)pParam;
	if (pLoader)
	{
		if (DownLoadHelper::Access("Version.xml",0) == 0)
		{
			//����
			//������
			DownLoadHelper::Rename("Version.xml","Version_old.xml");
			DownLoadHelper::Remove("Version.xml");
		}

		//Ȼ��
		pthread_mutex_lock(&pLoader->m_mutex);

		if (pLoader->m_bResDownloadFininsh)
		{
		}
		else
		{
			pthread_cond_wait(&pLoader->m_downloadappcond,&pLoader->m_mutex);
			while(!pLoader->m_bResDownloadFininsh)
			{
				DownLoadHelper::HttpSleep(1);
			}
		}
		pthread_mutex_unlock(&pLoader->m_mutex);
		if (1)
		{
			//�ȴ�����
			std::string url = pLoader->m_baseUrl + "app/Version.xml";
			bool bVersion = pLoader->DownloadFile("Version.xml",url,false);
			if (bVersion)
			{
				//ȥ����application
				std::string appexe;
				int nCurrentVersion = pLoader->AnalyzeVersion("Version.xml",appexe);
				if (nCurrentVersion > pLoader->m_nAppVersion)
				{
					url = pLoader->m_baseUrl + "app/" + appexe;

					//�ж���û�У��Լ����ܸ����Լ�
					std::string pathexe = appexe;
					if (DownLoadHelper::Access(appexe.c_str(),0) == 0)
					{
						//����
						//������
						int nPos = pathexe.rfind(".");
						if (nPos == std::string::npos)
						{
							pathexe.insert(nPos,"_old");
						}
						DownLoadHelper::Rename(appexe.c_str(),pathexe.c_str());
						DownLoadHelper::Remove(appexe.c_str());
					}

					//����app
#ifdef ANDROID
#elif IOS
#else
					pLoader->DownloadFile(appexe,url,false);
					pLoader->m_VersionAppName = appexe;
					pLoader->m_bInstall = true;
#endif
				}
				else if (pLoader->m_nAppVersion >= nCurrentVersion)
				{

				}

				//ɾ��oldversion
				DownLoadHelper::Remove("Version_old.xml");
			}
			pLoader->m_bExeDownloadFininsh = true;
		}
	}

	return NULL;
}

bool DownLoader::InstallApp()
{
	//���й���
	if (m_bInstall)
	{
		//��װ
		m_bInstall = false;

		return AppHelper::GetInstancePtr()->InstallApp(m_VersionAppName,"");
	}

	return false;
}

bool DownLoader::PrintDownList()
{
	for (int i = 0; i < m_vAlreadyFileList.size(); ++i)
	{
		std::pair<std::string,std::string> FileInfo = m_vAlreadyFileList.at(i);
		printf("down list :%s\n",FileInfo.first.c_str());
	}

	return true;
}

size_t DownLoader::GetContentLengthFunc( void *ptr, size_t size, size_t nmemb, void *stream )
{
	if (ptr == NULL || stream == NULL)
		return 0;
	int r;
	long len = 0;

	/* _snscanf() is Win32 specific */
	// r = _snscanf(ptr, size * nmemb, "Content-Length: %ld\n", &len);
	r = sscanf((char*)ptr, "Content-Length: %ld\n", &len);
	if (r) /* Microsoft: we don't read the specs */
		*((long *) stream) = len;

	return size * nmemb;
}
