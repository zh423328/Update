#include "AppHelper_Win32.h"
#include <Windows.h>

AppHelperWin32::AppHelperWin32()
{

}

AppHelperWin32::~AppHelperWin32()
{

}

AppHelper* AppHelperWin32::GetInstancePtr()
{
	if (m_pInstance == NULL)
		m_pInstance = new AppHelperWin32();

	return m_pInstance;
}

bool AppHelperWin32::InstallApp( std::string apppath,std::string url )
{
	WinExec(apppath.c_str(),SW_SHOW);
	return true;
}
