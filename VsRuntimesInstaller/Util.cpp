#include <Windows.h>
#include "Util.h"
/*
The default c run-time library has been disabled to avoid issues with any missing runtimes! 
Because of this only the WinAPI can be used.

Look at:
http://www.catch22.net/tuts/win32/reducing-executable-size
*/

void* Allocate( const unsigned int Bytes )
{
    return HeapAlloc( g_HeapHandle, HEAP_ZERO_MEMORY, (SIZE_T)Bytes );
}

void* Free( void* MemoryAddress )
{
    if ( MemoryAddress != NULL )
        HeapFree( g_HeapHandle, NULL, MemoryAddress );
    return NULL;
}

unsigned int _atoi(const char* InputString)
{
    unsigned int i, n;
    n = 0;
    for (i = 0; InputString[i] >= '0' && InputString[i] <= '9'; ++i)
        n = 10 * n + (InputString[i] - '0');
    return n;
}


char* _itoa( const unsigned int dwNumber, char* OutputString )
{
    unsigned int NbrCache = dwNumber;
    int cntr = 0;

    for ( unsigned int dwTemp = dwNumber; dwTemp > 0; cntr++, dwTemp/=10) ;

    OutputString[0] = 0;
    if ( cntr < 1 )
        return OutputString;

    for (int i = (cntr-1); i >= 0; i--)
    {
        char NewChar =  (char)(NbrCache%10) + (char)'0';
        NbrCache/=10; 
        OutputString[i] =  (unsigned char)( NewChar );
    }
    OutputString[cntr] = NULL;
    return OutputString;
}

unsigned int strlenA( const char* pString )
{
    unsigned int len = NULL;
    while (pString[len] != NULL){ len++; };
    return len;
}

unsigned int strlenW( const wchar_t* pString )
{
    unsigned int len = NULL;
    while (pString[len] != NULL){ len++; };
    return len;
}

char* strcpyA( char* dest, char* source )
{
    unsigned int i = NULL;
    for ( ; ; )
    {
        dest[ i ] = source[ i ];
        if (dest[ i ] == NULL) break;
        i++;
    }
    return dest;
}

void Print( char* Text )
{
    unsigned int NumberOfBytesWritten = NULL;
    WriteFile( g_StdOutputHandle, Text, (DWORD)strlenA( Text ), (DWORD*)&NumberOfBytesWritten, NULL );
}

wchar_t* StrDupW( const char* Str )
{
    unsigned int StringLenght = strlenA( Str ) + 1;
    wchar_t* StrW = (wchar_t*)Allocate( StringLenght * 2 );
    if ( StrW == NULL )
        return NULL;

    for ( unsigned int i = 0; i < StringLenght; i++)
        StrW[ i ] = (unsigned char)(Str[ i ]);
    return StrW;
};

wchar_t* StrAppendW( const wchar_t* Str1, wchar_t* Str2 )
{
    unsigned int StringLenght1 = strlenW( Str1 ) + 0;
    unsigned int StringLenght2 = strlenW( Str2 ) + 1;

    wchar_t* StrW = (wchar_t*)Allocate( StringLenght1 * 2 + StringLenght2 * 2 );
    if ( StrW == NULL )
        return NULL;

    for ( unsigned int i = 0; i < StringLenght1; i++)
        StrW[ i ] = Str1[ i ];

    for ( unsigned int j = 0; j < StringLenght2; j++)
        StrW[ StringLenght1 + j ] = Str2[ j ];

    return StrW;
};

wchar_t* StrPrependW( const wchar_t* Str1, wchar_t* Str2 )
{
    unsigned int StringLenght1 = strlenW( Str1 ) + 1;
    unsigned int StringLenght2 = strlenW( Str2 ) + 0;

    wchar_t* StrW = (wchar_t*)Allocate( StringLenght1 * 2 + StringLenght2 * 2 );
    if ( StrW == NULL )
        return NULL;

    for ( unsigned int i = 0; i < StringLenght2; i++)
        StrW[ i ] = Str2[ i ];

    for ( unsigned int j = 0; j < StringLenght1; j++)
        StrW[ StringLenght2 + j ] = Str1[ j ];

    return StrW;
};

wchar_t* StrPrependCurrentDirectory( const wchar_t* Str1 )
{
    wchar_t* Result = (wchar_t*)NULL;
    wchar_t* ModuleFileNameW = (wchar_t*)Allocate( (MAX_PATH+2) * 2 );
    if ( ModuleFileNameW == NULL )
        return NULL;

    if ( GetModuleFileNameW( NULL, ModuleFileNameW, MAX_PATH ) > 4 )
    {
        unsigned int Lenght = strlenW( ModuleFileNameW );
        for ( unsigned int i = Lenght - 1; i > 1 ; i--)
        {
            if ( ModuleFileNameW[i] == '\\' || ModuleFileNameW[i] == '/' )
            {
                ModuleFileNameW[i+1] = NULL;
                break;
            }
        }
        Result = StrPrependW( Str1, ModuleFileNameW );
    }
    else
    {
        if ( GetCurrentDirectoryW( MAX_PATH, ModuleFileNameW ) > 4 )
            Result = StrPrependW( Str1, ModuleFileNameW );
        else
            if ( GetTempPathW( MAX_PATH, ModuleFileNameW ) > 4 )
                Result = StrPrependW( Str1, ModuleFileNameW );
    }

    ModuleFileNameW = (wchar_t*)Free( ModuleFileNameW );
    return Result;
}

void* __cdecl _memset(void* src, int val, size_t count)
{
    char *char_src = (char *)src;

    while( count > 0 ) 
    {
        *char_src = val;
        char_src++;
        count--;
    }
    return src;
}

void ClearConsole()
{
    COORD topLeft;
    _memset( &topLeft, 0, sizeof(COORD) );

    CONSOLE_SCREEN_BUFFER_INFO screen;
    _memset( &screen, 0, sizeof(CONSOLE_SCREEN_BUFFER_INFO) );

    DWORD written = 0;

    GetConsoleScreenBufferInfo( g_StdOutputHandle, &screen );

    FillConsoleOutputCharacterA(
        g_StdOutputHandle, 
        ' ', 
        screen.dwSize.X * screen.dwSize.Y, 
        topLeft, 
        &written
        );

    FillConsoleOutputAttribute(
        g_StdOutputHandle, 
        screen.wAttributes,
        screen.dwSize.X * screen.dwSize.Y, 
        topLeft, 
        &written
        );
    SetConsoleCursorPosition(g_StdOutputHandle, topLeft);
}

Progressbar::Progressbar( /*IN*/ unsigned int Size )
{
    CONSOLE_SCREEN_BUFFER_INFO ConsoleScreenBufferInfo = {};
    _memset( &ConsoleScreenBufferInfo, 0, sizeof(CONSOLE_SCREEN_BUFFER_INFO) );

    GetConsoleScreenBufferInfo( g_StdOutputHandle, (CONSOLE_SCREEN_BUFFER_INFO*)&ConsoleScreenBufferInfo );

    this->m_CursorPosition = ConsoleScreenBufferInfo.dwCursorPosition;

    this->m_Size = Size;
}

void Progressbar::update( /*IN*/ double percentage )
{
    const BOOL ConsoleCursorPositionUpdated =
        SetConsoleCursorPosition( g_StdOutputHandle, this->m_CursorPosition );

    if ( ConsoleCursorPositionUpdated == TRUE )
    {

        unsigned int ProgressPercentage = (unsigned int)( (double)percentage * (double)(100.f)      );
        unsigned int ProgressSize       = (unsigned int)( (double)percentage * (double)this->m_Size );


        if ( ProgressSize > this->m_Size )
            ProgressSize = this->m_Size;

        char* ProgressBuffer = (char*)Allocate( this->m_Size + 100 );
        if ( ProgressBuffer == NULL )
        {
            Print("[Progressbar::update] Failed to allocate ProgressBuffer!\n");
            Sleep( 1000 );
            return;
        }

        ProgressBuffer[0] = (char)('[');

        for (unsigned int i = 0; i < this->m_Size; i++)
            ProgressBuffer[i+1] = (char)(' ');

        for (unsigned int i = 0; i < ProgressSize; i++)
            ProgressBuffer[i+1] = (char)('=');

        ProgressBuffer[this->m_Size+0] = (char)(']');
        ProgressBuffer[this->m_Size+1] = (char)(NULL);
        Print( ProgressBuffer );

        ProgressBuffer = (char*)Free( ProgressBuffer );

    }
}