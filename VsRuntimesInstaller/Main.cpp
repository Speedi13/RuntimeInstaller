#include <Windows.h>
#include "Util.h"
#include "RedistLibrary.h"
/*
The default c run-time library has been disabled to avoid issues with any missing runtimes! 
Because of this only the WinAPI can be used.

Look at:
http://www.catch22.net/tuts/win32/reducing-executable-size
*/

void PrintConsoleHeading();

HANDLE g_StdOutputHandle = INVALID_HANDLE_VALUE;
HANDLE g_HeapHandle = INVALID_HANDLE_VALUE;
HANDLE g_StdInputHandle = INVALID_HANDLE_VALUE;
HANDLE g_CurrentInstallerProcess = INVALID_HANDLE_VALUE;
HRESULT (STDAPICALLTYPE* fncURLDownloadToFileW)(LPUNKNOWN pCaller,LPCWSTR szURL,LPCWSTR szFileName,DWORD dwReserved,LPVOID lpfnCB) = NULL;

Progressbar* g_CurrentProgressBar = NULL;

BOOL WINAPI ConsoleCtrlHandler( DWORD CtrlType );

//https://github.com/reactos/reactos/tree/893a3c9d030fd8b078cbd747eeefd3f6ce57e560/sdk/lib/crt/misc
extern "C" int _fltused = 0x9875;

void * __cdecl operator new(size_t bytes){ return HeapAlloc(g_HeapHandle, NULL, bytes); }
void __cdecl operator delete(void *ptr){ if(ptr) HeapFree(g_HeapHandle, NULL, ptr); }



//Credits for the class below to https://stackoverflow.com/a/5292277
class DownloadProgress : public IBindStatusCallback {
public:
    HRESULT __stdcall QueryInterface(const IID &,void **) { 
        return E_NOINTERFACE;
    }
    ULONG STDMETHODCALLTYPE AddRef(void) { 
        return 1;
    }
    ULONG STDMETHODCALLTYPE Release(void) {
        return 1;
    }
    HRESULT STDMETHODCALLTYPE OnStartBinding(
            /* [in] */ DWORD dwReserved,
            /* [in] */ __RPC__in_opt IBinding *pib ) {
        return E_NOTIMPL;
    }
    virtual HRESULT STDMETHODCALLTYPE GetPriority( /* [out] */ __RPC__out LONG *pnPriority ) {
        return E_NOTIMPL;
    }
    virtual HRESULT STDMETHODCALLTYPE OnLowResource( /* [in] */ DWORD reserved ) {
        return S_OK;
    }
    virtual HRESULT STDMETHODCALLTYPE OnStopBinding(
            /* [in] */ HRESULT hresult,
            /* [unique][in] */ __RPC__in_opt LPCWSTR szError ) {
        return E_NOTIMPL;
    }
    virtual HRESULT STDMETHODCALLTYPE GetBindInfo(
            /* [out] */ DWORD *grfBINDF,
            /* [unique][out][in] */ BINDINFO *pbindinfo ) {
        return E_NOTIMPL;
    }
    virtual HRESULT STDMETHODCALLTYPE OnDataAvailable(
            /* [in] */ DWORD grfBSCF,
            /* [in] */ DWORD dwSize,
            /* [in] */ FORMATETC *pformatetc,
            /* [in] */ STGMEDIUM *pstgmed ) {
        return E_NOTIMPL;
    }        
    virtual HRESULT STDMETHODCALLTYPE OnObjectAvailable(
            /* [in] */ __RPC__in REFIID riid,
            /* [iid_is][in] */ __RPC__in_opt IUnknown *punk ) {
        return E_NOTIMPL;
    }

    virtual HRESULT __stdcall OnProgress(ULONG ulProgress, ULONG ulProgressMax, ULONG ulStatusCode, LPCWSTR szStatusText)
    {
        g_CurrentProgressBar->update( (float)ulProgress / (float)ulProgressMax );
        return S_OK;
    }
};

void InstallRedist( unsigned int Version, unsigned int Architecture, unsigned int Language )
{
    const RedistributableInformation* DownloadData = GetRedistributableInfo( Version, Architecture, Language );
    if ( DownloadData == NULL )
    {
        Print( "ERROR: No download data found!\n" );
        return;
    }

    const DownloadedFileName* FileNameInfo = GetRedistFileName( Version, Architecture, Language );
    if ( FileNameInfo == NULL )
    {
        Print( "ERROR: No file name info found!\n" );
        return;
    }

    const char* DownloadURL = DownloadData->DownloadURL;
    if ( DownloadURL == NULL )
    {
        Print( "ERROR: No download URL found!\n" );
        return;
    }

    const char* LocalFileName = FileNameInfo->LocalFileName;
    if ( LocalFileName == NULL )
    {
        Print( "ERROR: No local file name found!\n" );
        return;
    }

    wchar_t* WDownloadULR = StrDupW( DownloadURL  );
    wchar_t* WLocalFileName = StrDupW( LocalFileName );
    if ( WDownloadULR == NULL || WLocalFileName == NULL )
    {
        Print( "ERROR: Failed to allocate w-char buffer!\n" );

        if ( WDownloadULR != NULL )
             WDownloadULR = (wchar_t*)Free( WDownloadULR );

        if ( WLocalFileName != NULL )
             WLocalFileName = (wchar_t*)Free( WLocalFileName );
        return;
    }

    DeleteFileW( WLocalFileName );

    wchar_t* LocalFilePath = StrPrependCurrentDirectory( WLocalFileName );
    if ( LocalFilePath == NULL || LocalFilePath == WLocalFileName )
    {
        Print( "ERROR: Failed to prepend current directory!\n" );
        return;
    }

    DeleteFileW( LocalFilePath );

    DownloadProgress DownlodStatusCallback;
    Progressbar Progress( 78 );

    g_CurrentProgressBar = &Progress;

    SetLastError( ERROR_SUCCESS );

    HRESULT hr = fncURLDownloadToFileW( NULL,
                                        WDownloadULR, 
                                        LocalFilePath, 
                                        NULL,
                                        &DownlodStatusCallback );

    DWORD DownloadErrorCode = GetLastError( );

    g_CurrentProgressBar->update( (double)(1.f) );

    WLocalFileName = (wchar_t*)Free( WLocalFileName );
    WDownloadULR = (wchar_t*)Free( WDownloadULR );

    if ( hr != S_OK )
    {
        LocalFilePath = (wchar_t*)Free( LocalFilePath );
    
        char* Text = (char*)Allocate( 0x100 );
        _itoa( DownloadErrorCode, Text );
        
        if ( hr == E_OUTOFMEMORY )
            Print( "ERROR: E_OUTOFMEMORY [" );
        else
        if ( hr == INET_E_DOWNLOAD_FAILURE )
            Print( "ERROR: INET_E_DOWNLOAD_FAILURE [" );
        else
            Print( "ERROR: UNKNOWN ERROR [" );
        Print( Text );
        Print("]\n");

        Text = (char*)Free( Text );
        return;
    }

    Print("\nInstalling...\n");
    
    STARTUPINFOW StartupInfo;
    _memset( &StartupInfo, 0, sizeof(STARTUPINFOW) );

    StartupInfo.cb = sizeof(STARTUPINFOW);

    PROCESS_INFORMATION ProcessInfo;
    _memset( &ProcessInfo, 0, sizeof(PROCESS_INFORMATION) );

    WLocalFileName = StrDupW( LocalFileName );
    if ( WLocalFileName == NULL )
    {
        Print( "ERROR: Failed to allocate WLocalFileName!\n" );
        LocalFilePath = (wchar_t*)Free( LocalFilePath );
        Sleep( INFINITE );
        return;
    }

    wchar_t* ARG = L" /Q /norestart";
    if ( Version == RedistVer_2005 || Version == RedistVer_DirectX )
        ARG = L" /Q";

    Sleep( 1000 );

    wchar_t* StartParam = StrAppendW( LocalFilePath, ARG );

    LocalFilePath = (wchar_t*)Free( LocalFilePath );

    if ( StartParam == NULL )
    {
        Print( "ERROR: Failed to append start parameters!\n" );
        
        StartParam = (wchar_t*)Free( StartParam );
        return;
    }

    SetLastError( ERROR_SUCCESS );

    BOOL CreateProcessResult =
    CreateProcessW( NULL, (LPWSTR)StartParam, NULL, NULL, FALSE, HIGH_PRIORITY_CLASS | CREATE_NEW_CONSOLE, NULL, NULL, &StartupInfo, &ProcessInfo );
    
    const DWORD CreateProcessErrorCode = GetLastError();

    StartParam = (wchar_t*)Free( StartParam );

    g_CurrentInstallerProcess = ProcessInfo.hProcess;

    if ( CreateProcessResult != TRUE )
    {
        Print("CreateProcessW failed [");
        
        char* Text = (char*)Allocate( 0x100 );
        _itoa( CreateProcessErrorCode, Text );
        Print(Text);

        Text = (char*)Free( Text );
        Print("]\n");
    }
    else
        WaitForSingleObject( ProcessInfo.hProcess, INFINITE );

    Sleep( 1000 );

    if ( ProcessInfo.hProcess != NULL && ProcessInfo.hProcess != INVALID_HANDLE_VALUE )
        CloseHandle( ProcessInfo.hProcess );

    if ( ProcessInfo.hThread != NULL && ProcessInfo.hThread != INVALID_HANDLE_VALUE )
        CloseHandle( ProcessInfo.hThread  );

    g_CurrentInstallerProcess = INVALID_HANDLE_VALUE;
}

int start()
{
    //////////////////////////////////////////////////// Resolve Imports ////////////////////////////////////////////////////
    const HMODULE Urlmon = LoadLibraryW( L"urlmon.dll" );
    if ( Urlmon != NULL )
        fncURLDownloadToFileW = ( decltype(fncURLDownloadToFileW) )GetProcAddress( Urlmon, "URLDownloadToFileW" );
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////// Initialize APIs /////////////////////////////////////////////////////
    //Setup Heap
    g_HeapHandle = HeapCreate( NULL, NULL, NULL );

    //Open console window:
    AllocConsole();

    //Get handle to console output:
    g_StdOutputHandle = GetStdHandle( STD_OUTPUT_HANDLE );
    g_StdInputHandle  = GetStdHandle( STD_INPUT_HANDLE );
    
    unsigned int dwConsoleMode = 0;
    if ( GetConsoleMode( g_StdInputHandle, (DWORD*)&dwConsoleMode ) == TRUE )
    {
        dwConsoleMode &= (unsigned int)(~(ENABLE_QUICK_EDIT_MODE));
        SetConsoleMode( g_StdInputHandle, dwConsoleMode );
    }
    SetConsoleCtrlHandler( ConsoleCtrlHandler, TRUE );
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    BOOLEAN Success = FALSE;

    PrintConsoleHeading();

    if ( g_HeapHandle == NULL || g_HeapHandle == INVALID_HANDLE_VALUE )
        Print("ERROR: couldn't create heap!\n");
    else
    if ( g_StdOutputHandle == NULL || g_StdOutputHandle == INVALID_HANDLE_VALUE )
        Print("ERROR: couldn't get a valid StdOutputHandle\n");
    else
    if ( Urlmon == NULL )
        Print("ERROR: couldn't find the library \"urlmon.dll\"!\n");
    else
    if ( fncURLDownloadToFileW == NULL )
        Print("ERROR: couldn't find the function \"URLDownloadToFileW\" inside the library \"urlmon.dll\"\n");
    else
    {
        unsigned int RedistLang = RedistLang_enUS;

        Print(
        "*******************************************\n"
        "*            1  -  English                *\n"
        "*            2  -  Chinese (Simplified)   *\n"
        "*            3  -  Chinese (Traditional)  *\n"
        "*            4  -  French                 *\n"
        "*            5  -  German                 *\n"
        "*            6  -  Italian                *\n"
        "*            7  -  Japanese               *\n"
        "*            8  -  Korean                 *\n"
        "*            9  -  Spanish                *\n"
        "*******************************************\n"
        "\n" );

        Print("Please select a language: ");

        CHAR AsciiChar = NULL;
        for ( ;  AsciiChar == NULL ;  )
        {
            DWORD dwEventCount = NULL;
            if ( GetNumberOfConsoleInputEvents( g_StdInputHandle , &dwEventCount ) != TRUE ) break;
            if ( dwEventCount == NULL )
            {
                Sleep( 200 );
                continue;
            }
            INPUT_RECORD *NewEvents = (INPUT_RECORD*)Allocate( ( dwEventCount + 1 ) * sizeof(INPUT_RECORD) );

            DWORD NumberOfEventsRead = NULL;
            if ( ReadConsoleInputA( g_StdInputHandle, NewEvents, dwEventCount, &NumberOfEventsRead ) == TRUE )
            {
                if ( NumberOfEventsRead > dwEventCount )
                     NumberOfEventsRead = dwEventCount;

                for (DWORD u = 0; u < NumberOfEventsRead; u++)
                {
                    INPUT_RECORD* Event = &NewEvents[u];
                    if ( Event->EventType == KEY_EVENT )
                    {
                        AsciiChar = Event->Event.KeyEvent.uChar.AsciiChar;
                        u = NumberOfEventsRead;
                    }
                }

            }
            NewEvents = (INPUT_RECORD*)Free( NewEvents );
            
            bool IsValidNumber = (AsciiChar >= '1' && AsciiChar <= '9');
            if ( IsValidNumber )
            {
                RedistLang = AsciiChar - '1';
            }
            else
            {
                if ( AsciiChar == 'e' || AsciiChar == 'E' )
                    RedistLang = RedistLang_enUS;
                else
                if ( AsciiChar == 'c' || AsciiChar == 'C' )
                    RedistLang = RedistLang_zhCN;
                else
                if ( AsciiChar == 'f' || AsciiChar == 'F' )
                    RedistLang = RedistLang_frFR;
                else
                if ( AsciiChar == 'g' || AsciiChar == 'G' )
                    RedistLang = RedistLang_deDE;
                else
                if ( AsciiChar == 'i' || AsciiChar == 'I' )
                    RedistLang = RedistLang_itIT;
                else
                if ( AsciiChar == 'j' || AsciiChar == 'J' )
                    RedistLang = RedistLang_jaJP;
                else
                if ( AsciiChar == 'k' || AsciiChar == 'K' )
                    RedistLang = RedistLang_koKR;
                else
                if ( AsciiChar == 's' || AsciiChar == 'S' )
                    RedistLang = RedistLang_esES;
                else
                    AsciiChar = NULL;
            }
        }
        char Entered[3] = { AsciiChar, '\n', 0 };
        Print(Entered);

        ClearConsole();
        PrintConsoleHeading();

        Print("You selected ");
        Print(RedistributableLanguageToString( RedistLang ) );
        Print("\n");
        Sleep( 2000 );
        const unsigned int length = sizeof(g_RedistVersions) / sizeof(RedistributableVersion);

        for ( unsigned int i = 0; i < length; i++)
        {
            const RedistributableVersion Version = g_RedistVersions[i];
            Print("\nDownloading ");
            Print( RedistributableVersionToString( Version ) );
            Print(" (X86)\n");
            InstallRedist( Version, RedistArch_X86, RedistLang );

            Print("\nDownloading ");
            Print( RedistributableVersionToString( Version ) );
            Print(" (X64)\n");
            InstallRedist( Version, RedistArch_X64, RedistLang );
        }

        Print("\nDownloading ");
        Print( RedistributableVersionToString( RedistVer_DirectX ) );
        Print("\n");
        InstallRedist( RedistVer_DirectX, RedistArch_X86, RedistLang_enUS );
        Success = TRUE;
    }

    if ( Success == TRUE )
        MessageBoxW( NULL, L"Done :D", L"Runtime Installer", MB_ICONINFORMATION | MB_SETFOREGROUND );
    else
        MessageBoxW( NULL, L"An error has occured!", NULL /*ERROR*/, MB_ICONERROR | MB_SETFOREGROUND );

    ///////////////////////////////////////////////////// Release APIs ///////////////////////////////////////////////////////
    FreeConsole();
    if ( g_HeapHandle != NULL && g_HeapHandle != INVALID_HANDLE_VALUE )
        HeapDestroy( g_HeapHandle );
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////// Release Imports /////////////////////////////////////////////////////
    fncURLDownloadToFileW = NULL;
    if ( Urlmon != NULL )
        FreeLibrary( Urlmon );
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    ExitProcess( ERROR_SUCCESS );
    return 0;
}

DownloadedFileName* GetRedistFileName( unsigned int Version, unsigned int Architecture, unsigned int Language )
{
    unsigned int Size = sizeof(g_RedistFileNames) / sizeof(DownloadedFileName);
    for (unsigned int i = 0; i < Size; i++)
    {
        if ( g_RedistFileNames[i].Version != Version )
            continue;
        if ( g_RedistFileNames[i].Architecture != Architecture )
            continue;

        return (DownloadedFileName*)&g_RedistFileNames[i];
    }
    return NULL;
}

RedistributableInformation* GetRedistributableInfo( unsigned int Version, unsigned int Architecture, unsigned int Language )
{
    unsigned int RedistLibrarySize = sizeof(g_RedistLibrary) / sizeof(RedistributableInformation);

    for (unsigned int i = 0; i < RedistLibrarySize; i++)
    {
        if ( g_RedistLibrary[i].Version != Version )
            continue;
        if ( g_RedistLibrary[i].Architecture != Architecture )
            continue;
        if ( g_RedistLibrary[i].Language != Language )
            continue;

        return (RedistributableInformation*)&g_RedistLibrary[i];
    }
    if ( Language == RedistLang_enUS )
        return NULL;

    return GetRedistributableInfo( Version, Architecture, RedistLang_enUS );
}


char* RedistributableVersionToString( unsigned int v )
{
    switch (v)
    {
    case RedistVer_2005:
        return "Microsoft Visual C++ 2005 Redistributable Package";
        break;
    case RedistVer_2008:
        return "Microsoft Visual C++ 2008 Redistributable Package";
        break;
    case RedistVer_2010:
        return "Microsoft Visual C++ 2010 Redistributable Package";
        break;
    case RedistVer_2012:
        return "Microsoft Visual C++ 2012 Redistributable Package";
        break;
    case RedistVer_2013:
        return "Microsoft Visual C++ 2013 Redistributable Package";
        break;
    case RedistVer_2015:
        return "Microsoft Visual C++ 2015 Redistributable Package";
        break;
    case RedistVer_2017:
        return "Microsoft Visual C++ 2017 Redistributable Package";
        break;
    case RedistVer_2019:
        return "Microsoft Visual C++ 2019 Redistributable Package";
        break;
    case RedistVer_DirectX:
        return "DirectX End-User Runtime";
        break;
    default:
        return "UNKNOWN";
        break;
    }
}

char* RedistributableLanguageToString( unsigned int l )
{
    switch (l)
    {
    case RedistLang_enUS:
        return "English";
        
    case RedistLang_zhCN:
        return "Chinese (Simplified)";
        
    case RedistLang_zhTW:
        return "Chinese (Traditional)";
        
    case RedistLang_frFR:
        return "French";
        
    case RedistLang_deDE:
        return "German";
        
    case RedistLang_itIT:
        return "Italian";
        
    case RedistLang_jaJP:
        return "Japanese";
        
    case RedistLang_koKR:
        return "Korean";
        
    case RedistLang_esES:
        return "Spanish";
        
    default:
        break;
    }
    return "ERROR";
}

//https://docs.microsoft.com/en-us/windows/console/handlerroutine
BOOL WINAPI ConsoleCtrlHandler( DWORD CtrlType )
{
    switch ( CtrlType )
    {
    case CTRL_C_EVENT:
    case CTRL_CLOSE_EVENT:
    case CTRL_LOGOFF_EVENT:
    case CTRL_SHUTDOWN_EVENT:
        {
            if ( g_CurrentInstallerProcess != NULL && g_CurrentInstallerProcess != INVALID_HANDLE_VALUE )
            {
                TerminateProcess( g_CurrentInstallerProcess, ERROR_SUCCESS );
                CloseHandle( g_CurrentInstallerProcess );
            }
            g_CurrentInstallerProcess = INVALID_HANDLE_VALUE;
            return TRUE;
        }
    default:
        break;
    }
    return FALSE;
};

void PrintConsoleHeading()
{
    Print(
        "**********************************************************\n"
        "*             Runtime downloader by Speedi13             *\n"
        "**********************************************************\n"
        "\n"
        );
}