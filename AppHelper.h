#ifndef APPHELP_H_
#define APPHELP_H_
//////////////////////////////////////////////////////////////////////////
// app ��װƽ̨������ƽ̨����
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


	//��װ�µ�app������ƽ̨��д
	//appname ����app��λ�ã�win32��Ч
	//url://url����·������װios,����itenus
	virtual bool InstallApp(std::string apppath,std::string url);		//url����·������װios
protected:
	static AppHelper *m_pInstance;
};

#endif