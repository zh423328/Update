#ifndef APPHELP_ANDROID_H_
#define APPHELP_ANDROID_H_

//android调用
#include "AppHelper.h"

class AppHelperAndroid : public AppHelper
{
public:
	AppHelperAndroid();
	virtual ~AppHelperAndroid();

	static AppHelper* GetInstancePtr();

	//安装新的app，各大平台重写
	//appname 下载app的位置，win32有效, android apk ios ipa
	//url://url配置路径，安装ios,访问itenus
	virtual bool InstallApp(std::string apppath,std::string url);		
};

#endif