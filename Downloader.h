#ifndef DOWNLOADER_H_
#define DOWNLOADER_H_

//暂时就用一条线程来进行下载

#include <pthread.h>
#include <string>
#include <vector>

#define CC_BREAK_IF(cond)            if(cond) break

class DownLoadHelper
{
public:
	//拆分
	static std::vector<std::string> split(std::string str, std::string separator);  

	//去除首尾空格
	//
	static std::string& trim(std::string &s);

	//获取文件md5
	static std::string FileMd5(std::string filename);


	//创建文件
	static bool MakeDir(const char* path);

	//创建连续文件夹
	static void CreateDir(std::string filepath);

	//获取运行时间
	static unsigned int xnGetTickCount();

	//获取本地文件大小
	static unsigned int GetLocalFileSize(std::string filepath);

	//判断文件或文件夹是否存在
	static int Access(const char * szFile,int mode);

	//移除文件
	static int Remove(const char * szFile);

	//重命名文件
	static int Rename(const char *oldname,const char *newname);

	//sleep
	static void HttpSleep(int nMillsec);
};

//资料路径
//baseurl/resources/各种资料 + md5文件 filelist

//apk 路径
//baseurl/apk/XXX.apk + version.xml

//下载
class DownLoader
{
public:
	DownLoader();
	~DownLoader();

	//下载总的列表
	virtual bool DownFileList();
	//打印列表
	virtual bool PrintDownList();	
	//分析文件结构
	virtual bool AnalyzeFileList();			//分析文件

	//开启下载线程
	bool StartDownloadThread();				//开启下载线程

	//解析version
	int AnalyzeVersion(char * szVersionFileName,std::string &fileApp);					//解析version
	
	//基础路径
	virtual bool SetBastUrl(std::string baseUrl);

	//查看是否下载完成
	virtual bool GetDownloadFinish()			{ return m_bResDownloadFininsh&&m_bExeDownloadFininsh;}

	//Download resources,文件名字
	//bAdd = false重写
	virtual bool DownloadFile(std::string filename,std::string fileUrlList,bool bAdd);

	//安装app
	virtual bool InstallApp();

	//下载线程
	static void * Worker(void *param);
	static void * DownloadApp(void *pParam);		
	//写数据
	static size_t writefunc(void *ptr, size_t size, size_t nmemb, void *stream);

	//回调
	static int progress_callback(void *clientp,   double dltotal,   double dlnow,   double ultotal,   double ulnow);

	//获取文件大小
	static size_t GetContentLengthFunc(void *ptr, size_t size, size_t nmemb, void *stream);
private:
	std::vector<std::string> m_filelist;			//已经下载的文件
	std::vector<std::pair<std::string,std::string> > m_vAlreadyFileList;	//准备的要下载的路径

	pthread_t		m_downloadthread;					//下载线程
	pthread_t		m_downloadappthread;				//downloadapp
	pthread_cond_t	m_downloadappcond;					//app cond
	pthread_mutex_t m_mutex;							//mutex

	std::string m_baseUrl;							//基础url

	//值判断
	bool	m_bResDownloadFininsh;					//资源下载完成
	bool	m_bExeDownloadFininsh;					//可执行程序exe,apk,ipa
	int		m_nAppVersion;							//version
	std::string m_VersionAppName;						//versionAppName;	//.exe

	bool	m_bInstall;								//是否需要重新安装
};
//下载管理
#endif