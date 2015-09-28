#include "AppHelper_IOS.h"
#include <Windows.h>

AppHelperIOS::AppHelperIOS()
{

}

AppHelperIOS::~AppHelperIOS()
{

}

AppHelper* AppHelperIOS::GetInstancePtr()
{
	if (m_pInstance == NULL)
		m_pInstance = new AppHelperIOS();

	return m_pInstance;
}

//安装ipa
//url为：itms-services://?action=download-manifest&url=plist
//plist:为更新服务器端存放plist路径
//<?xml version="1.0" encoding="UTF-8"?>
/*<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
   <key>items</key>
   <array>
       <dict>
           <key>assets</key>
           <array>
               <dict>
                   <key>kind</key>
                   <string>software-package</string>
                   <key>url</key>
				   <-- ipa 地址 -->
                   <string>http://download.176sy.com/pack/m_client/atl_client.ipa</string>
               </dict>
               <dict>  
                    <key>kind</key>  
                    <string>full-size-image</string>  
                    <key>needs-shine</key>  
                    <true/>  
                    <key>url</key>  
					<-- icon -->
                    <string>http://cw.176sy.com/pcgw/images/Icon-72.tiff</string>  
               </dict> 
               <dict>
                   <key>kind</key>
                   <string>display-image</string>
                   <key>needs-shine</key>
                   <true/>
                   <key>url</key>
				   <-- icon -->
			         <string>http://cw.176sy.com/pcgw/images/Icon-72.tiff</string>
               </dict>
           </array>
           <key>metadata</key>
           <dict>
		   <-- 弹出框 提示安装 -->
               <key>bundle-identifier</key>
               <string>com.zhangdong.aotianlu.official</string>
               <key>bundle-version</key>
               <string>1.0</string>
               <key>kind</key>
               <string>software</string>
               <key>subtitle</key>
               <string>掌动科技</string>
               <key>title</key>
               <string>傲天录</string>
           </dict>
       </dict>
   </array>
</dict>
</plist>*/
//
///
//
//
//
//
//
//
///
//
//
//
//
//
//
//
//
//
//
//
bool AppHelperIOS::InstallApp( std::string apppath,std::string url )
{
	[[UIApplication shareApplication] openURL:[NSURL URLWithString:[NSString stringWithFormat:@"%s", url.c_str()]]];
	return true;
}
