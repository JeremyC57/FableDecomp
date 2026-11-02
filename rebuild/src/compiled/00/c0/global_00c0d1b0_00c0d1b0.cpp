// CString::FormatV(const char*, va_list) -- size and format a variable-argument string
// Leaf: ?FormatV@CString@@QAEXPBDPAD@Z
#include "engine/CString.h"
#include <malloc.h>
extern "C" {
__declspec(dllimport) unsigned char* __cdecl _mbsinc(const unsigned char*);
__declspec(dllimport) int __cdecl _mbclen(const unsigned char*);
__declspec(dllimport) int __cdecl _ismbcdigit(unsigned int);
__declspec(dllimport) int __cdecl _mbsnbcmp(const unsigned char*,const unsigned char*,size_t);
__declspec(dllimport) int __cdecl atoi(const char*);
__declspec(dllimport) size_t __cdecl wcslen(const unsigned short*);
__declspec(dllimport) int __stdcall lstrlenA(const char*);
__declspec(dllimport) int __cdecl sprintf(char*,const char*,...);
__declspec(dllimport) int __cdecl vsprintf(char*,const char*,va_list);
size_t __cdecl strlen(const char*);
}
#pragma intrinsic(strlen)
#define _tcsinc(x) ((const char*)_mbsinc((const unsigned char*)(x)))
#define max(a,b) (((a)>(b))?(a):(b))
#define min(a,b) (((a)<(b))?(a):(b))
#define _T(x) x
#define FALSE 0
#define _ATL_USE_CSTRING_FLOAT
typedef const char* LPCTSTR;
typedef const char* LPCSTR;
typedef char* LPTSTR;
typedef unsigned short* LPWSTR;
void CString::FormatV(const char* lpszFormat, va_list argList)
{
	LIB_ASSERT(IsValidString(lpszFormat));

	enum _FormatModifiers
	{
		FORCE_ANSI =	0x10000,
		FORCE_UNICODE =	0x20000,
		FORCE_INT64 =	0x40000
	};

	va_list argListSave;
#ifdef _MSC_VER
    argListSave = argList;
#else
    __va_copy(argListSave, argList);
#endif

	// make a guess at the maximum length of the resulting string
	int nMaxLen = 0;
	for (LPCTSTR lpsz = lpszFormat; *lpsz != '\0'; lpsz = _tcsinc(lpsz))
	{
		// handle '%' character, but watch out for '%%'
		if (*lpsz != '%' || *(lpsz = _tcsinc(lpsz)) == '%')
		{
			nMaxLen += _mbclen((const unsigned char*)lpsz);
			continue;
		}

		int nItemLen = 0;

		// handle '%' character with format
		int nWidth = 0;
		for (; *lpsz != '\0'; lpsz = _tcsinc(lpsz))
		{
			// check for valid flags
			if (*lpsz == '#')
				nMaxLen += 2;   // for '0x'
			else if (*lpsz == '*')
				nWidth = va_arg(argList, int);
			else if (*lpsz == '-' || *lpsz == '+' || *lpsz == '0' || *lpsz == ' ')
				;
			else // hit non-flag character
				break;
		}
		// get width and skip it
		if (nWidth == 0)
		{
			// width indicated by
			nWidth = atoi(lpsz);
			for (; *lpsz != '\0' && _ismbcdigit(*lpsz); lpsz = _tcsinc(lpsz))
				;
		}
		LIB_ASSERT(nWidth >= 0);

		int nPrecision = 0;
		if (*lpsz == '.')
		{
			// skip past '.' separator (width.precision)
			lpsz = _tcsinc(lpsz);

			// get precision and skip it
			if (*lpsz == '*')
			{
				nPrecision = va_arg(argList, int);
				lpsz = _tcsinc(lpsz);
			}
			else
			{
				nPrecision = atoi(lpsz);
				for (; *lpsz != '\0' && _ismbcdigit(*lpsz); lpsz = _tcsinc(lpsz))
					;
			}
			LIB_ASSERT(nPrecision >= 0);
		}

		// should be on type modifier or specifier
		int nModifier = 0;
		if (_mbsnbcmp((const unsigned char*)lpsz,(const unsigned char*)"I64",3)==0)
		{
			lpsz += 3;
			nModifier = FORCE_INT64;
		}
		else
		{
			switch (*lpsz)
			{
			// modifiers that affect size
			case 'h':
				nModifier = FORCE_ANSI;
				lpsz = _tcsinc(lpsz);
				break;
			case 'l':
				nModifier = FORCE_UNICODE;
				lpsz = _tcsinc(lpsz);
				break;

			// modifiers that do not affect size
			case 'F':
			case 'N':
			case 'L':
				lpsz = _tcsinc(lpsz);
				break;
			}
		}

		// now should be on specifier
		switch (*lpsz | nModifier)
		{
		// single characters
		case 'c':
		case 'C':
			nItemLen = 2;
			va_arg(argList, int);
			break;
		case 'c' | FORCE_ANSI:
		case 'C' | FORCE_ANSI:
			nItemLen = 2;
			va_arg(argList, int);
			break;
		case 'c' | FORCE_UNICODE:
		case 'C' | FORCE_UNICODE:
			nItemLen = 2;
			va_arg(argList, int);
			break;

		// strings
		case 's':
		{
			LPCTSTR pstrNextArg = va_arg(argList, LPCTSTR);
			if (pstrNextArg == NULL)
			{
				nItemLen = 6;  // "(null)"
			}
			else
			{
				nItemLen = lstrlenA(pstrNextArg);
				nItemLen = max(1, nItemLen);
			}
			break;
		}

		case 'S':
		{
#ifndef _UNICODE
			LPWSTR pstrNextArg = va_arg(argList, LPWSTR);
			if (pstrNextArg == NULL)
			{
				nItemLen = 6;  // "(null)"
			}
			else
			{
				nItemLen = (int)wcslen(pstrNextArg);
				nItemLen = max(1, nItemLen);
			}
#else
			LPCSTR pstrNextArg = va_arg(argList, LPCSTR);
			if (pstrNextArg == NULL)
			{
				nItemLen = 6; // "(null)"
			}
			else
			{
				nItemLen = lstrlenA(pstrNextArg);
				nItemLen = max(1, nItemLen);
			}
#endif
			break;
		}

		case 's' | FORCE_ANSI:
		case 'S' | FORCE_ANSI:
		{
			LPCSTR pstrNextArg = va_arg(argList, LPCSTR);
			if (pstrNextArg == NULL)
			{
				nItemLen = 6; // "(null)"
			}
			else
			{
				nItemLen = lstrlenA(pstrNextArg);
				nItemLen = max(1, nItemLen);
			}
			break;
		}

		case 's' | FORCE_UNICODE:
		case 'S' | FORCE_UNICODE:
		{
			LPWSTR pstrNextArg = va_arg(argList, LPWSTR);
			if (pstrNextArg == NULL)
			{
				nItemLen = 6; // "(null)"
			}
			else
			{
				nItemLen = (int)wcslen(pstrNextArg);
				nItemLen = max(1, nItemLen);
			}
			break;
		}
		}

		// adjust nItemLen for strings
		if (nItemLen != 0)
		{
			if (nPrecision != 0)
				nItemLen = min(nItemLen, nPrecision);
			nItemLen = max(nItemLen, nWidth);
		}
		else
		{
			switch (*lpsz)
			{
			// integers
			case 'd':
			case 'i':
			case 'u':
			case 'x':
			case 'X':
			case 'o':
				if (nModifier & FORCE_INT64)
					va_arg(argList, __int64);
				else
					va_arg(argList, int);
				nItemLen = 32;
				nItemLen = max(nItemLen, nWidth + nPrecision);
				break;

#ifndef _ATL_USE_CSTRING_FLOAT
			case 'e':
			case 'f':
			case 'g':
			case 'G':
				LIB_ASSERT(!"Floating point (%%e, %%f, %%g, and %%G) is not supported by the WTL::CString class.");
#ifndef _DEBUG
				::OutputDebugString(_T("Floating point (%%e, %%f, %%g, and %%G) is not supported by the WTL::CString class."));
				::DebugBreak();
#endif //!_DEBUG
				break;
#else //_ATL_USE_CSTRING_FLOAT
			case 'e':
			case 'g':
			case 'G':
				va_arg(argList, double);
				nItemLen = 128;
				nItemLen = max(nItemLen, nWidth + nPrecision);
				break;
			case 'f':
				{
					double f;
					LPTSTR pszTemp;

					// 312 == strlen("-1+(309 zeroes).")
					// 309 zeroes == max precision of a double
					// 6 == adjustment in case precision is not specified,
					//   which means that the precision defaults to 6
					pszTemp = (LPTSTR)_alloca(max(nWidth, 312 + nPrecision + 6));

					f = va_arg(argList, double);
					sprintf(pszTemp, _T( "%*.*f" ), nWidth, nPrecision + 6, f);
					nItemLen = strlen(pszTemp);
				}
				break;
#endif //_ATL_USE_CSTRING_FLOAT

			case 'p':
				va_arg(argList, void*);
				nItemLen = 32;
				nItemLen = max(nItemLen, nWidth + nPrecision);
				break;

			// no output
			case 'n':
				va_arg(argList, int*);
				break;

			default:
				LIB_ASSERT(FALSE);  // unknown formatting option
			}
		}

		// adjust nMaxLen for output nItemLen
		nMaxLen += nItemLen;
	}

	GetBuffer(nMaxLen);
    if (!(vsprintf(m_pchData,lpszFormat,argListSave)<=GetAllocLength())) {
        if (g_pfnAssertHook) g_pfnAssertHook("simple verify failed\n");
        CDebugBreak();
    }
    ReleaseBuffer();
    va_end(argListSave);
}