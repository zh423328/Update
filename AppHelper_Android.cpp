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

//����android ����
bool AppHelperAndroid::InstallApp( std::string apppath,std::string url )
{
	//WinExec(apppath.c_str(),SW_SHOW);
	//����android ����
	//�������ʵ�־�������jni ����java����ķ���
	//
	//����ʵ��
	/*File apkfile = new File(apk_file_name);
	if (!apkfile.exists())
	{
		return;
	}
	// ͨ��Intent��װAPK�ļ�
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
