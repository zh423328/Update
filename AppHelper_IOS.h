#ifndef APPHELP_IOS_H_
#define APPHELP_IOS_H_

//android����
#include "AppHelper.h"

class AppHelperIOS : public AppHelper
{
public:
	AppHelperIOS();
	virtual ~AppHelperIOS();

	static AppHelper* GetInstancePtr();

	//��װ�µ�app������ƽ̨��д
	//appname ����app��λ�ã�win32��Ч, android apk ios ipa
	//url://url����·������װios,����itenus
	virtual bool InstallApp(std::string apppath,std::string url);		
};

#endif