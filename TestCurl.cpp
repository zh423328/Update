// TestCurl.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Downloader.h"



int _tmain(int argc, _TCHAR* argv[])
{
	//CURLcode res;
	//res = curl_global_init(CURL_GLOBAL_WIN32);		//全局初始化

	//if (CURLE_OK != res)
	//{
	//	printf("init curl failed!");
	//	return -1;
	//}

	//time_t timer = time(NULL);
	//tm *now = localtime(&timer);		//当前事件

	//char szTmp[128] = {0};
	//std::string random;
	//sprintf(szTmp,"%04d%02d%02d%02d%02d%02d",now->tm_year+1900,now->tm_mon+1,now->tm_mday,now->tm_hour,now->tm_min,now->tm_sec);
	////std::string random = DateTime.Now.ToString("yyyymmddhhmmss");
	//random = szTmp;
	//std::string baseUrl = "http://localhost:280/u3dhttp/simpleframework/";
	//std::string listUrl = "http://localhost:280/u3dhttp/simpleframework/files.txt?v=" + random;

	//if (_access("files.txt",0) == 0)
	//{	//有文件，先删除
	//	remove("files.txt");
	//}
	//DownLoadFile("files.txt",listUrl.c_str());

	////读取文件
	//FILE * fp = fopen("files.txt","rb");
	//if (fp == NULL)
	//	return -1;

	////读取保存
	//std::vector<std::string> files;		//文件列表

	//char szLine[1024] = {0};
	////是否是结尾
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

	//		//判断文件是否需要下载
	//		bool bUpdate = false;			
	//		int nPos =  filename.find_last_of("/");
	//		if (nPos != std::string::npos)
	//		{
	//			//不是结尾，或者不存在
	//			std::string path = filename.substr(0,nPos);

	//			if (!path.empty())
	//			{
	//				//如果不存在文件夹时创建文件
	//				if (::access(path.c_str(),0) != 0)	//access跨平台
	//				{
	//					//创建文件
	//					//mkdir(path.c_str());
	//					CreateDir(path);
	//				}

	//				//然后判断是否需要更新
	//			}
	//		}

	//		//判断md5 
	//		if (_access(filename.c_str(),0) == 0)
	//		{
	//			//存在判断md5
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
	//			//更新
	//			listUrl = baseUrl + filename + "v?=" + random;

	//			DownLoadFile(filename.c_str(),listUrl.c_str());
	//		}
	//	}
	//}

	//curl_global_cleanup();

	DownLoader *loader  = new DownLoader();
	loader->SetBastUrl("http://10.10.10.82:8080/u3dhttp/simpleframework/");
	bool bStart = loader->DownFileList();	//下载文件

	if (bStart)
	{
		while(!loader->GetDownloadFinish())
		{
			;
		}

		printf("下载完成！\n");

		//安装文件
		if (loader->InstallApp())
		{
			exit(0);
		}
	}
	else
	{
		printf("下载失败！\n");
	}
	system("pause");

	return 0;
}

