#ifndef APPHELP_H_
#define APPHELP_H_
//////////////////////////////////////////////////////////////////////////
// app 安装平台，各大平台重载
//
//////////////////////////////////////////////////////////////////////////
#include <string>

class AppHelper
{
public:
	AppHelper();
	virtual ~AppHelper();

	static AppHelper* GetInstancePtr();

	static void purge();


	//安装新的app，各大平台重写
	//appname 下载app的位置，win32有效
	//url://url配置路径，安装ios,访问itenus
	virtual bool InstallApp(std::string apppath,std::string url);		//url配置路径，安装ios
protected:
	static AppHelper *m_pInstance;
};

#endif