#ifndef DOWNLOADER_H_
#define DOWNLOADER_H_

//��ʱ����һ���߳�����������

#include <pthread.h>
#include <string>
#include <vector>

#define CC_BREAK_IF(cond)            if(cond) break

class DownLoadHelper
{
public:
	//���
	static std::vector<std::string> split(std::string str, std::string separator);  

	//ȥ����β�ո�
	//
	static std::string& trim(std::string &s);

	//��ȡ�ļ�md5
	static std::string FileMd5(std::string filename);


	//�����ļ�
	static bool MakeDir(const char* path);

	//���������ļ���
	static void CreateDir(std::string filepath);

	//��ȡ����ʱ��
	static unsigned int xnGetTickCount();

	//��ȡ�����ļ���С
	static unsigned int GetLocalFileSize(std::string filepath);

	//�ж��ļ����ļ����Ƿ����
	static int Access(const char * szFile,int mode);

	//�Ƴ��ļ�
	static int Remove(const char * szFile);

	//�������ļ�
	static int Rename(const char *oldname,const char *newname);

	//sleep
	static void HttpSleep(int nMillsec);
};

//����·��
//baseurl/resources/�������� + md5�ļ� filelist

//apk ·��
//baseurl/apk/XXX.apk + version.xml

//����
class DownLoader
{
public:
	DownLoader();
	~DownLoader();

	//�����ܵ��б�
	virtual bool DownFileList();
	//��ӡ�б�
	virtual bool PrintDownList();	
	//�����ļ��ṹ
	virtual bool AnalyzeFileList();			//�����ļ�

	//���������߳�
	bool StartDownloadThread();				//���������߳�

	//����version
	int AnalyzeVersion(char * szVersionFileName,std::string &fileApp);					//����version
	
	//����·��
	virtual bool SetBastUrl(std::string baseUrl);

	//�鿴�Ƿ��������
	virtual bool GetDownloadFinish()			{ return m_bResDownloadFininsh&&m_bExeDownloadFininsh;}

	//Download resources,�ļ�����
	//bAdd = false��д
	virtual bool DownloadFile(std::string filename,std::string fileUrlList,bool bAdd);

	//��װapp
	virtual bool InstallApp();

	//�����߳�
	static void * Worker(void *param);
	static void * DownloadApp(void *pParam);		
	//д����
	static size_t writefunc(void *ptr, size_t size, size_t nmemb, void *stream);

	//�ص�
	static int progress_callback(void *clientp,   double dltotal,   double dlnow,   double ultotal,   double ulnow);

	//��ȡ�ļ���С
	static size_t GetContentLengthFunc(void *ptr, size_t size, size_t nmemb, void *stream);
private:
	std::vector<std::string> m_filelist;			//�Ѿ����ص��ļ�
	std::vector<std::pair<std::string,std::string> > m_vAlreadyFileList;	//׼����Ҫ���ص�·��

	pthread_t		m_downloadthread;					//�����߳�
	pthread_t		m_downloadappthread;				//downloadapp
	pthread_cond_t	m_downloadappcond;					//app cond
	pthread_mutex_t m_mutex;							//mutex

	std::string m_baseUrl;							//����url

	//ֵ�ж�
	bool	m_bResDownloadFininsh;					//��Դ�������
	bool	m_bExeDownloadFininsh;					//��ִ�г���exe,apk,ipa
	int		m_nAppVersion;							//version
	std::string m_VersionAppName;						//versionAppName;	//.exe

	bool	m_bInstall;								//�Ƿ���Ҫ���°�װ
};
//���ع���
#endif