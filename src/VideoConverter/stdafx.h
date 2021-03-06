#pragma once

#ifdef _DEBUG
#define DEBUG 1
#endif

#pragma warning( disable : 4995 )

#if defined(NDEBUG) && defined(_DEBUG)
#error ?
#endif

#ifdef WINDOWS_XP
#define  _WIN32_WINNT   0x0501
#else
#define _WIN32_WINNT	0x0600
#endif

#define INITGUID				1
#define _CRT_SECURE_NO_WARNINGS 1
#define GDIPVER					0x0200
#define BUILD_WINDOWS			1

// Windows 头文件:
#include <guiddef.h>
#include <SDKDDKVer.h>
#include <windows.h>
#include <windowsx.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <tchar.h>
#include <shlobj.h>
#include <shlwapi.h>
#include <strsafe.h>
#include <gdiplus.h>
#include <gdiplus.h>
#include <Mshtml.h>
#include <MMSystem.h>
#include <d3d9.h>
#include <DSound.h>
#include <assert.h>

using namespace Gdiplus;

extern HINSTANCE	g_hInstance;
extern HANDLE		g_hJob;
extern wchar_t		g_szModulePath[MAX_PATH];
extern wchar_t		g_szTempPath[MAX_PATH];

#include "../Include/ProfileManager.h"
#include "XDebug.h"

#ifdef _DEBUG
#define BREAK() DebugBreak()
#else 
#define BREAK() 
#endif

#ifdef _DEBUG
static inline HRESULT _E_NOTIMPL_RESULT(char* file, int32_t line)
{
	if (IsDebuggerPresent())
	{
		DebugBreak();
	}
	else
	{
		ErrorLog("Not implemented!!, File:%hs, Line:%d\n", file, line); 
	}
	return E_NOTIMPL; 
}
#define E_NOTIMPL_RESULT() _E_NOTIMPL_RESULT(__FILE__, __LINE__) 
#else
#define E_NOTIMPL_RESULT() E_NOTIMPL
#endif

#ifdef _DEBUG
static inline HRESULT _E_INVALIDARG_RESULT(char* file, int32_t line)
{
	if (IsDebuggerPresent())
	{
		DebugBreak();
	}
	else
	{
		ErrorLog("Arguments are invalid!!, File:%hs, Line:%d\n", file, line); 
	}
	return E_INVALIDARG; 
}
#define E_INVALIDARG_RESULT() _E_INVALIDARG_RESULT(__FILE__, __LINE__) 
#else
#define E_INVALIDARG_RESULT() E_INVALIDARG
#endif

#ifdef _DEBUG
static inline HRESULT _E_FAIL_RESULT(char* file, int32_t line)
{
	if (IsDebuggerPresent())
	{
		DebugBreak();
	}
	else
	{
		ErrorLog("An internal error!!, File:%hs, Line:%d\n", file, line); 
	}
	return E_FAIL; 
}
#define E_FAIL_RESULT() _E_FAIL_RESULT(__FILE__, __LINE__) 
#else
#define E_FAIL_RESULT() E_FAIL
#endif

#define JIF(x) if (FAILED(hr=(x))) \
	{ \
		ErrorLog("Failed(0x%x) on %hs, FILE:%s, LINE:%d\n", hr, #x, __FILE__, __LINE__); \
		return hr; \
	}

#define JIZ(x) if (FALSE == (x)) \
	{ \
		ErrorLog("Failed(%d) on %hs, FILE:%s, LINE:%d\n", GetLastError(), #x, __FILE__, __LINE__); \
		return E_FAIL;\
	}

#define malloc	error_malloc
#define free	error_free
#define realloc error_realloc

static __inline void *MemoryAlloc(int size)
{
	return _aligned_malloc(size, 32);
}

static __inline void *MemoryRealloc(void *p, int size)
{
	return _aligned_realloc(p, size, 32);
}

static __inline void MemoryFree(void *p)
{
	_aligned_free(p);
}

static __inline void* operator new( size_t cb )
{
	void *p = MemoryAlloc(cb);
	ZeroMemory(p, cb);
	return p;
}

static __inline void operator delete( void *p )
{
	MemoryFree(p);
}

#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }
#define SAFE_CLOSE(h) if (h){CloseHandle(h); h = NULL;}
#define SAFE_DELETE(p) if (p){delete p; p = NULL;}
#define SAFE_FREE(p) if (p){MemoryFree(p); p = NULL;}
#define SAFE_UVF(p) if (p){UnmapViewOfFile(p); p = NULL;}
#define SAFE_FREE_BSTR(str) if (str){ SysFreeString(str); str = NULL; }
