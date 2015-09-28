#ifndef APPHELP_WIN32_H_
#define APPHELP_WIN32_H_

#include "AppHelper.h"

class AppHelperWin32:public AppHelper
{
public:
	AppHelperWin32();
	virtual ~AppHelperWin32();

	static AppHelper* GetInstancePtr();

	//安装新的app，各大平台重写
	//appname 下载app的位置，win32有效
	//url://url配置路径，安装ios,访问itenus
	virtual bool InstallApp(std::string apppath,std::string url);		
};
#endif