#pragma once

enum RedistributableVersion
{
	RedistVer_2005,
	RedistVer_2008,
	RedistVer_2010,
	RedistVer_2012,
	RedistVer_2013,
	RedistVer_2015,
	RedistVer_2017,
	RedistVer_2019,

	RedistVer_DirectX,
};

char* RedistributableVersionToString( unsigned int );

static const RedistributableVersion g_RedistVersions[] =
{
	RedistVer_2005,
	RedistVer_2008,
	RedistVer_2010,
	RedistVer_2012,
	RedistVer_2013,
	RedistVer_2015,
	RedistVer_2017,
	RedistVer_2019,
};

enum RedistributableArchitecture
{
	RedistArch_X86,
	RedistArch_X64,
};

enum RedistributableLanguage
{
	RedistLang_enUS, //English
	RedistLang_zhCN, //Chinese (Simplified)
	RedistLang_zhTW, //Chinese (Traditional)
	RedistLang_frFR, //French
	RedistLang_deDE, //German
	RedistLang_itIT, //Italian
	RedistLang_jaJP, //Japanese
	RedistLang_koKR, //Korean
	RedistLang_esES, //Spanish

};
char* RedistributableLanguageToString( unsigned int );

typedef struct _RedistributableInformation
{
	RedistributableVersion Version;
	RedistributableArchitecture Architecture;
	RedistributableLanguage Language;
	char* DownloadURL;
} RedistributableInformation;

typedef struct _DownloadedFileName
{
	RedistributableVersion Version;
	RedistributableArchitecture Architecture;
	char* LocalFileName;
} DownloadedFileName;

RedistributableInformation* GetRedistributableInfo( unsigned int Version, unsigned int Architecture, unsigned int Language );

static const
DownloadedFileName g_RedistFileNames[] = 
{
	{ RedistVer_2005, RedistArch_X86, "2005_vcredist_x86.exe" },
	{ RedistVer_2005, RedistArch_X64, "2005_vcredist_x64.exe" },

	{ RedistVer_2008, RedistArch_X86, "2008_vcredist_x86.exe" },
	{ RedistVer_2008, RedistArch_X64, "2008_vcredist_x64.exe" },

	{ RedistVer_2010, RedistArch_X86, "2010_vcredist_x86.exe" },
	{ RedistVer_2010, RedistArch_X64, "2010_vcredist_x64.exe" },

	{ RedistVer_2012, RedistArch_X86, "2012_vcredist_x86.exe" },
	{ RedistVer_2012, RedistArch_X64, "2012_vcredist_x64.exe" },

	{ RedistVer_2013, RedistArch_X86, "2013_vcredist_x86.exe" },
	{ RedistVer_2013, RedistArch_X64, "2013_vcredist_x64.exe" },

	{ RedistVer_2015, RedistArch_X86, "2015_vcredist_x86.exe" },
	{ RedistVer_2015, RedistArch_X64, "2015_vcredist_x64.exe" },

	{ RedistVer_2017, RedistArch_X86, "2017_vcredist_x86.exe" },
	{ RedistVer_2017, RedistArch_X64, "2017_vcredist_x64.exe" },

	{ RedistVer_2019, RedistArch_X86, "2019_vcredist_x86.exe" },
	{ RedistVer_2019, RedistArch_X64, "2019_vcredist_x64.exe" },

	{ RedistVer_DirectX, RedistArch_X86, "DirectX_redist.exe" },
};

DownloadedFileName* GetRedistFileName( unsigned int Version, unsigned int Architecture, unsigned int Language );

static const
RedistributableInformation g_RedistLibrary[] =
{
	//////////////////////////////////////// 2005 ////////////////////////////////////////
	//Microsoft Visual C++ 2005 Service Pack 1 Redistributable Package MFC Security Update 
	//Date Published: 6/7/2011
	//=> https://www.microsoft.com/en-us/download/details.aspx?id=26347

	{RedistVer_2005, RedistArch_X86, RedistLang_enUS, "https://download.microsoft.com/download/8/B/4/8B42259F-5D70-43F4-AC2E-4B208FD8D66A/vcredist_x86.EXE" },
	{RedistVer_2005, RedistArch_X64, RedistLang_enUS, "https://download.microsoft.com/download/8/B/4/8B42259F-5D70-43F4-AC2E-4B208FD8D66A/vcredist_x64.EXE" },

	{RedistVer_2005, RedistArch_X86, RedistLang_zhCN, "https://download.microsoft.com/download/4/A/2/4A22001F-FA3B-4C13-BF4E-42EC249D51C4/vcredist_x86.EXE" },
	{RedistVer_2005, RedistArch_X64, RedistLang_zhCN, "https://download.microsoft.com/download/4/A/2/4A22001F-FA3B-4C13-BF4E-42EC249D51C4/vcredist_x64.EXE" },

	{RedistVer_2005, RedistArch_X86, RedistLang_zhTW, "https://download.microsoft.com/download/1/1/3/1131962C-70D7-41B8-B093-7B215D192619/vcredist_x86.EXE" },
	{RedistVer_2005, RedistArch_X64, RedistLang_zhTW, "https://download.microsoft.com/download/1/1/3/1131962C-70D7-41B8-B093-7B215D192619/vcredist_x64.EXE" },

	{RedistVer_2005, RedistArch_X86, RedistLang_frFR, "https://download.microsoft.com/download/5/D/A/5DA273D6-C1CB-4F1C-90C0-73B5263E0AC7/vcredist_x86.EXE" },
	{RedistVer_2005, RedistArch_X64, RedistLang_frFR, "https://download.microsoft.com/download/5/D/A/5DA273D6-C1CB-4F1C-90C0-73B5263E0AC7/vcredist_x64.EXE" },

	{RedistVer_2005, RedistArch_X86, RedistLang_deDE, "https://download.microsoft.com/download/1/6/7/167A16B9-61F1-4DCF-8E59-3A60D38507CA/vcredist_x86.EXE" },
	{RedistVer_2005, RedistArch_X64, RedistLang_deDE, "https://download.microsoft.com/download/1/6/7/167A16B9-61F1-4DCF-8E59-3A60D38507CA/vcredist_x64.EXE" },

	{RedistVer_2005, RedistArch_X86, RedistLang_itIT, "https://download.microsoft.com/download/3/7/5/3755033D-C8D8-4B88-9DC9-B666BEA33349/vcredist_x86.EXE" },
	{RedistVer_2005, RedistArch_X64, RedistLang_itIT, "https://download.microsoft.com/download/3/7/5/3755033D-C8D8-4B88-9DC9-B666BEA33349/vcredist_x64.EXE" },

	{RedistVer_2005, RedistArch_X86, RedistLang_jaJP, "https://download.microsoft.com/download/E/A/5/EA5A3F64-0347-404B-8E26-3067207C8FA2/vcredist_x86.EXE" },
	{RedistVer_2005, RedistArch_X64, RedistLang_jaJP, "https://download.microsoft.com/download/E/A/5/EA5A3F64-0347-404B-8E26-3067207C8FA2/vcredist_x64.EXE" },

	{RedistVer_2005, RedistArch_X86, RedistLang_koKR, "https://download.microsoft.com/download/1/E/4/1E4D029E-1D34-4CA8-B269-2CFEB91BD066/vcredist_x86.EXE" },
	{RedistVer_2005, RedistArch_X64, RedistLang_koKR, "https://download.microsoft.com/download/1/E/4/1E4D029E-1D34-4CA8-B269-2CFEB91BD066/vcredist_x64.EXE" },

	{RedistVer_2005, RedistArch_X86, RedistLang_esES, "https://download.microsoft.com/download/5/3/7/5379AA9F-6172-4A56-A968-6FC80C9A201D/vcredist_x86.EXE" },
	{RedistVer_2005, RedistArch_X64, RedistLang_esES, "https://download.microsoft.com/download/5/3/7/5379AA9F-6172-4A56-A968-6FC80C9A201D/vcredist_x64.EXE" },

	//////////////////////////////////////// 2005 ////////////////////////////////////////
	//Microsoft Visual C++ 2008 Service Pack 1 Redistributable Package MFC Security Update
	//Date Published: 6/7/2011
	//=> https://www.microsoft.com/en-us/download/details.aspx?id=26368

	//=> Same download for all Languages:
	{RedistVer_2008, RedistArch_X86, RedistLang_enUS, "https://download.microsoft.com/download/5/D/8/5D8C65CB-C849-4025-8E95-C3966CAFD8AE/vcredist_x86.exe" },
	{RedistVer_2008, RedistArch_X64, RedistLang_enUS, "https://download.microsoft.com/download/5/D/8/5D8C65CB-C849-4025-8E95-C3966CAFD8AE/vcredist_x64.exe" },


	//////////////////////////////////////// 2010 ////////////////////////////////////////
	//Microsoft Visual C++ 2010 Service Pack 1 Redistributable Package MFC Security Update
	//Date Published: 8/9/2011
	//=> https://www.microsoft.com/en-us/download/details.aspx?id=26999

	//=> Same download for all Languages:
	{RedistVer_2010, RedistArch_X86, RedistLang_enUS, "https://download.microsoft.com/download/1/6/5/165255E7-1014-4D0A-B094-B6A430A6BFFC/vcredist_x86.exe" },
	{RedistVer_2010, RedistArch_X64, RedistLang_enUS, "https://download.microsoft.com/download/1/6/5/165255E7-1014-4D0A-B094-B6A430A6BFFC/vcredist_x64.exe" },


	//////////////////////////////////////// 2012 ////////////////////////////////////////
	//Visual C++ Redistributable for Visual Studio 2012 Update 4 
	//Date Published: 11/20/2013
	//=> https://www.microsoft.com/en-us/download/details.aspx?id=30679

	{RedistVer_2012, RedistArch_X86, RedistLang_enUS, "https://download.microsoft.com/download/1/6/B/16B06F60-3B20-4FF2-B699-5E9B7962F9AE/VSU_4/vcredist_x86.exe" },
	{RedistVer_2012, RedistArch_X64, RedistLang_enUS, "https://download.microsoft.com/download/1/6/B/16B06F60-3B20-4FF2-B699-5E9B7962F9AE/VSU_4/vcredist_x64.exe" },

	{RedistVer_2012, RedistArch_X86, RedistLang_zhCN, "https://download.microsoft.com/download/9/C/D/9CD480DC-0301-41B0-AAAB-FE9AC1F60237/VSU4/vcredist_x86.exe" },
	{RedistVer_2012, RedistArch_X64, RedistLang_zhCN, "https://download.microsoft.com/download/9/C/D/9CD480DC-0301-41B0-AAAB-FE9AC1F60237/VSU4/vcredist_x64.exe" },

	{RedistVer_2012, RedistArch_X86, RedistLang_zhTW, "https://download.microsoft.com/download/6/3/C/63C9B770-2D4B-4B9A-B057-3D610C57CE44/VSU4/vcredist_x86.exe" },
	{RedistVer_2012, RedistArch_X64, RedistLang_zhTW, "https://download.microsoft.com/download/6/3/C/63C9B770-2D4B-4B9A-B057-3D610C57CE44/VSU4/vcredist_x64.exe" },

	{RedistVer_2012, RedistArch_X86, RedistLang_frFR, "https://download.microsoft.com/download/D/3/B/D3B72629-7D95-49ED-A4EC-7FF105754124/VSU4/vcredist_x86.exe" },
	{RedistVer_2012, RedistArch_X64, RedistLang_frFR, "https://download.microsoft.com/download/D/3/B/D3B72629-7D95-49ED-A4EC-7FF105754124/VSU4/vcredist_x64.exe" },

	{RedistVer_2012, RedistArch_X86, RedistLang_deDE, "https://download.microsoft.com/download/A/3/7/A371A2C7-B787-4AD9-B56D-8319CE7B40CA/VSU4/vcredist_x86.exe" },
	{RedistVer_2012, RedistArch_X64, RedistLang_deDE, "https://download.microsoft.com/download/A/3/7/A371A2C7-B787-4AD9-B56D-8319CE7B40CA/VSU4/vcredist_x64.exe" },

	{RedistVer_2012, RedistArch_X86, RedistLang_itIT, "https://download.microsoft.com/download/1/3/2/13297EFB-ED35-46B5-BD9A-F32CF7CC2CFF/VSU4/vcredist_x86.exe" },
	{RedistVer_2012, RedistArch_X64, RedistLang_itIT, "https://download.microsoft.com/download/1/3/2/13297EFB-ED35-46B5-BD9A-F32CF7CC2CFF/VSU4/vcredist_x64.exe" },

	{RedistVer_2012, RedistArch_X86, RedistLang_jaJP, "https://download.microsoft.com/download/C/A/F/CAF5E118-4803-4D68-B6B5-A1772903D119/VSU4/vcredist_x86.exe" },
	{RedistVer_2012, RedistArch_X64, RedistLang_jaJP, "https://download.microsoft.com/download/C/A/F/CAF5E118-4803-4D68-B6B5-A1772903D119/VSU4/vcredist_x64.exe" },

	{RedistVer_2012, RedistArch_X86, RedistLang_koKR, "https://download.microsoft.com/download/0/D/8/0D8C2D7C-75DD-409D-B70A-FDC0953343C1/VSU4/vcredist_x86.exe" },
	{RedistVer_2012, RedistArch_X64, RedistLang_koKR, "https://download.microsoft.com/download/0/D/8/0D8C2D7C-75DD-409D-B70A-FDC0953343C1/VSU4/vcredist_x64.exe" },

	{RedistVer_2012, RedistArch_X86, RedistLang_esES, "https://download.microsoft.com/download/D/C/C/DCCCFB1C-B0E0-43D6-9623-D6300DBDAF0C/VSU4/vcredist_x86.exe" },
	{RedistVer_2012, RedistArch_X64, RedistLang_esES, "https://download.microsoft.com/download/D/C/C/DCCCFB1C-B0E0-43D6-9623-D6300DBDAF0C/VSU4/vcredist_x64.exe" },

	//////////////////////////////////////// 2013 ////////////////////////////////////////
	//Visual C++ 2013 Redistributable Package Update 5
	//Date Published: 07/18/2017
	//=> https://support.microsoft.com/en-us/help/4032938/update-for-visual-c-2013-redistributable-package

	{RedistVer_2013, RedistArch_X86, RedistLang_enUS, "https://download.visualstudio.microsoft.com/download/pr/10912113/5da66ddebb0ad32ebd4b922fd82e8e25/vcredist_x86.exe" },
	{RedistVer_2013, RedistArch_X64, RedistLang_enUS, "https://download.visualstudio.microsoft.com/download/pr/10912041/cee5d6bca2ddbcd039da727bf4acb48a/vcredist_x64.exe" },

	{RedistVer_2013, RedistArch_X86, RedistLang_zhCN, "https://download.visualstudio.microsoft.com/download/pr/10912119/33c0eec4a4c7aa0e2d7891f5237e5890/vcredist_x86.exe" },
	{RedistVer_2013, RedistArch_X64, RedistLang_zhCN, "https://download.visualstudio.microsoft.com/download/pr/10912040/8a9bc6a378988031b4de96bf6eecbe37/vcredist_x64.exe" },

	{RedistVer_2013, RedistArch_X86, RedistLang_zhTW, "https://download.visualstudio.microsoft.com/download/pr/10912111/2914f54fe016843f175da631f2d9df4b/vcredist_x86.exe" },
	{RedistVer_2013, RedistArch_X64, RedistLang_zhTW, "https://download.visualstudio.microsoft.com/download/pr/10912039/4070ad50e8bc0f9c3934124458567157/vcredist_x64.exe" },

	{RedistVer_2013, RedistArch_X86, RedistLang_frFR, "https://download.visualstudio.microsoft.com/download/pr/10912123/d33248bbcb2283f685638d73c3d3091b/vcredist_x86.exe" },
	{RedistVer_2013, RedistArch_X64, RedistLang_frFR, "https://download.visualstudio.microsoft.com/download/pr/10912044/ada72838034ef323cead551238be4a69/vcredist_x64.exe" },

	{RedistVer_2013, RedistArch_X86, RedistLang_deDE, "https://download.visualstudio.microsoft.com/download/pr/10912120/c186b17b78fcbf682de47acfc41aeba0/vcredist_x86.exe" },
	{RedistVer_2013, RedistArch_X64, RedistLang_deDE, "https://download.visualstudio.microsoft.com/download/pr/10912036/b519cb85bab02255e6e00e4562942748/vcredist_x64.exe" },

	{RedistVer_2013, RedistArch_X86, RedistLang_itIT, "https://download.visualstudio.microsoft.com/download/pr/10912116/c705d418ea5aa13f738dcf72e6018346/vcredist_x86.exe" },
	{RedistVer_2013, RedistArch_X64, RedistLang_itIT, "https://download.visualstudio.microsoft.com/download/pr/10912051/809f519e20771c8640a25f0d3973ac99/vcredist_x64.exe" },

	{RedistVer_2013, RedistArch_X86, RedistLang_jaJP, "https://download.visualstudio.microsoft.com/download/pr/10912115/88b8f4396e3edaa58c1350db34cdd751/vcredist_x86.exe" },
	{RedistVer_2013, RedistArch_X64, RedistLang_jaJP, "https://download.visualstudio.microsoft.com/download/pr/10912049/3831217601a8a928c5119d826bafd8f7/vcredist_x64.exe" },

	{RedistVer_2013, RedistArch_X86, RedistLang_koKR, "https://download.visualstudio.microsoft.com/download/pr/10912122/d8838d40e3897340c9802748fa52da67/vcredist_x86.exe" },
	{RedistVer_2013, RedistArch_X64, RedistLang_koKR, "https://download.visualstudio.microsoft.com/download/pr/10912050/03b459d0af335835aff5eb4a3b0ea2ff/vcredist_x64.exe" },

	{RedistVer_2013, RedistArch_X86, RedistLang_esES, "https://download.visualstudio.microsoft.com/download/pr/10912112/4f9686d96ce0bce352d2fe20430fa5ce/vcredist_x86.exe" },
	{RedistVer_2013, RedistArch_X64, RedistLang_esES, "https://download.visualstudio.microsoft.com/download/pr/10912042/4963615feae8448392140e34e8a32eba/vcredist_x64.exe" },

	//////////////////////////////////////// 2015 ////////////////////////////////////////
	//Microsoft Visual C++ 2015 Redistributable Update 3 
	//Date Published: 8/18/2016
	//=> https://www.microsoft.com/en-us/download/details.aspx?id=53587

	//=> Same download for all Languages:
	{RedistVer_2015, RedistArch_X86, RedistLang_enUS, "https://download.microsoft.com/download/6/D/F/6DF3FF94-F7F9-4F0B-838C-A328D1A7D0EE/vc_redist.x86.exe" },
	{RedistVer_2015, RedistArch_X64, RedistLang_enUS, "https://download.microsoft.com/download/6/D/F/6DF3FF94-F7F9-4F0B-838C-A328D1A7D0EE/vc_redist.x64.exe" },

	//////////////////////////////////////// 2017 ////////////////////////////////////////
	//Microsoft Visual C++ Redistributable for Visual Studio 2017
	//=> https://visualstudio.microsoft.com/vs/older-downloads/

	//=> Same download for all Languages:
	{RedistVer_2017, RedistArch_X86, RedistLang_enUS, "https://aka.ms/vs/15/release/VC_redist.x86.exe" },
	{RedistVer_2017, RedistArch_X64, RedistLang_enUS, "https://aka.ms/vs/15/release/VC_redist.x64.exe" },

	//////////////////////////////////////// 2019 ////////////////////////////////////////
	//Microsoft Visual C++ Redistributable for Visual Studio 2019
	//=> https://visualstudio.microsoft.com/downloads/

	//=> Same download for all Languages:
	{RedistVer_2019, RedistArch_X86, RedistLang_enUS, "https://aka.ms/vs/16/release/VC_redist.x86.exe" },
	{RedistVer_2019, RedistArch_X64, RedistLang_enUS, "https://aka.ms/vs/16/release/VC_redist.x64.exe" },

	/////////////////////////////////////// DirectX //////////////////////////////////////
	//DirectX End-User Runtime Web Installer
	//=> https://www.microsoft.com/en-us/download/details.aspx?id=35

	{RedistVer_DirectX, RedistArch_X86, RedistLang_enUS, "https://download.microsoft.com/download/1/7/1/1718CCC4-6315-4D8E-9543-8E28A4E18C4C/dxwebsetup.exe" },
	
};
