// TestCurl.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Downloader.h"



int _tmain(int argc, _TCHAR* argv[])
{
	//CURLcode res;
	//res = curl_global_init(CURL_GLOBAL_WIN32);		//ȫ�ֳ�ʼ��

	//if (CURLE_OK != res)
	//{
	//	printf("init curl failed!");
	//	return -1;
	//}

	//time_t timer = time(NULL);
	//tm *now = localtime(&timer);		//��ǰ�¼�

	//char szTmp[128] = {0};
	//std::string random;
	//sprintf(szTmp,"%04d%02d%02d%02d%02d%02d",now->tm_year+1900,now->tm_mon+1,now->tm_mday,now->tm_hour,now->tm_min,now->tm_sec);
	////std::string random = DateTime.Now.ToString("yyyymmddhhmmss");
	//random = szTmp;
	//std::string baseUrl = "http://localhost:280/u3dhttp/simpleframework/";
	//std::string listUrl = "http://localhost:280/u3dhttp/simpleframework/files.txt?v=" + random;

	//if (_access("files.txt",0) == 0)
	//{	//���ļ�����ɾ��
	//	remove("files.txt");
	//}
	//DownLoadFile("files.txt",listUrl.c_str());

	////��ȡ�ļ�
	//FILE * fp = fopen("files.txt","rb");
	//if (fp == NULL)
	//	return -1;

	////��ȡ����
	//std::vector<std::string> files;		//�ļ��б�

	//char szLine[1024] = {0};
	////�Ƿ��ǽ�β
	//while(!feof(fp))
	//{
	//	fgets(szLine,1024,fp);
	//	std::string tmp = szLine;
	//	files.push_back(szLine);
	//}

	//for (int i = 0; i < files.size(); ++i)
	//{
	//	std::string file = files[i];

	//	std::vector<std::string> keyValue = split(file,"|");
	//	if (keyValue.size()>= 2)
	//	{
	//		std::string filename = keyValue[0];
	//		std::string md5Value = keyValue[1];
	//		filename = trim(filename);
	//		md5Value = trim(md5Value);

	//		//�ж��ļ��Ƿ���Ҫ����
	//		bool bUpdate = false;			
	//		int nPos =  filename.find_last_of("/");
	//		if (nPos != std::string::npos)
	//		{
	//			//���ǽ�β�����߲�����
	//			std::string path = filename.substr(0,nPos);

	//			if (!path.empty())
	//			{
	//				//����������ļ���ʱ�����ļ�
	//				if (::access(path.c_str(),0) != 0)	//access��ƽ̨
	//				{
	//					//�����ļ�
	//					//mkdir(path.c_str());
	//					CreateDir(path);
	//				}

	//				//Ȼ���ж��Ƿ���Ҫ����
	//			}
	//		}

	//		//�ж�md5 
	//		if (_access(filename.c_str(),0) == 0)
	//		{
	//			//�����ж�md5
	//			std::string filemd5 = FileMd5(filename);
	//			bool bUpdate = (filemd5 == md5Value);	

	//			if (bUpdate)
	//				remove(filename.c_str());
	//		}
	//		else
	//		{
	//			bUpdate = true;
	//		}

	//		if (bUpdate)
	//		{
	//			//����
	//			listUrl = baseUrl + filename + "v?=" + random;

	//			DownLoadFile(filename.c_str(),listUrl.c_str());
	//		}
	//	}
	//}

	//curl_global_cleanup();

	DownLoader *loader  = new DownLoader();
	loader->SetBastUrl("http://10.10.10.82:8080/u3dhttp/simpleframework/");
	bool bStart = loader->DownFileList();	//�����ļ�

	if (bStart)
	{
		while(!loader->GetDownloadFinish())
		{
			;
		}

		printf("������ɣ�\n");

		//��װ�ļ�
		if (loader->InstallApp())
		{
			exit(0);
		}
	}
	else
	{
		printf("����ʧ�ܣ�\n");
	}
	system("pause");

	return 0;
}

