#ifndef APPHELP_ANDROID_H_
#define APPHELP_ANDROID_H_

//android����
#include "AppHelper.h"

class AppHelperAndroid : public AppHelper
{
public:
	AppHelperAndroid();
	virtual ~AppHelperAndroid();

	static AppHelper* GetInstancePtr();

	//��װ�µ�app������ƽ̨��д
	//appname ����app��λ�ã�win32��Ч, android apk ios ipa
	//url://url����·������װios,����itenus
	virtual bool InstallApp(std::string apppath,std::string url);		
};

#endif