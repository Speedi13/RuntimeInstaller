#pragma once

void* Allocate( const unsigned int Bytes );
void* Free( void* MemoryAddress );

unsigned int _atoi(const char* InputString);

char* _itoa( const unsigned int dwNumber, char* OutputString );

unsigned int strlenA( const char* pString );
unsigned int strlenW( const wchar_t* pString );

char* strcpyA( char* dest, char* source );

wchar_t* StrDupW( const char* Str );

wchar_t* StrAppendW( const wchar_t* Str1, wchar_t* Str2 );
wchar_t* StrPrependW( const wchar_t* Str1, wchar_t* Str2 );

wchar_t* StrPrependCurrentDirectory( const wchar_t* Str1 );

void* __cdecl _memset(void* src, int val, size_t count);

void Print( char* Text );

void ClearConsole();

class Progressbar
{
private:
	COORD m_CursorPosition;
	unsigned int m_Size;
public:
	Progressbar( /*IN*/ unsigned int Size );
	void update( /*IN*/ double percentage );
};

extern HANDLE g_StdOutputHandle;
extern HANDLE g_HeapHandle;