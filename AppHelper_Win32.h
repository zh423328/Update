#ifndef APPHELP_WIN32_H_
#define APPHELP_WIN32_H_

#include "AppHelper.h"

class AppHelperWin32:public AppHelper
{
public:
	AppHelperWin32();
	virtual ~AppHelperWin32();

	static AppHelper* GetInstancePtr();

	//��װ�µ�app������ƽ̨��д
	//appname ����app��λ�ã�win32��Ч
	//url://url����·������װios,����itenus
	virtual bool InstallApp(std::string apppath,std::string url);		
};
#endif