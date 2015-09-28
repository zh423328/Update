#include "AppHelper_Android.h"

AppHelperAndroid::AppHelperAndroid()
{

}

AppHelperAndroid::~AppHelperAndroid()
{

}

AppHelper* AppHelperAndroid::GetInstancePtr()
{
	if (m_pInstance == NULL)
		m_pInstance = new AppHelperAndroid();

	return m_pInstance;
}

//调用android 方法
bool AppHelperAndroid::InstallApp( std::string apppath,std::string url )
{
	//WinExec(apppath.c_str(),SW_SHOW);
	//调用android 方法
	//这里具体实现就是利用jni 调用java里面的方法
	//
	//具体实现
	/*File apkfile = new File(apk_file_name);
	if (!apkfile.exists())
	{
		return;
	}
	// 通过Intent安装APK文件
	Intent i = new Intent(Intent.ACTION_VIEW);
	i.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
	i.setDataAndType(Uri.parse("file://" + apk_file_name), "application/vnd.android.package-archive");
	sContext.startActivity(i);
	android.os.Process.killProcess(android.os.Process.myPid());*/
	//
	//
	//////////////////////////////////////////////////////////////////////////
	return true;
}
