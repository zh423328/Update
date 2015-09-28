//application
#include "stdafx.h"
#include "AppHelper.h"

AppHelper * AppHelper::m_pInstance = NULL;


AppHelper::AppHelper()
{

}

AppHelper::~AppHelper()
{

}

AppHelper* AppHelper::GetInstancePtr()
{
	if (m_pInstance == NULL)
		m_pInstance = new AppHelper();

	return m_pInstance;
}

bool AppHelper::InstallApp( std::string apppath,std::string url )
{
	return true;
}

void AppHelper::purge()
{
	SAFE_DELETE(m_pInstance);
}
