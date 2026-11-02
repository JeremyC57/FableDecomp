#ifndef _MSC_VER
#define __cdecl
#define __stdcall
#define __fastcall
#define __int64 long
#define _alloca __builtin_alloca
#endif
#define __declspec(x)
// CString::FormatV(const char*, va_list) -- size and format a variable-argument string
// Leaf: ?FormatV@CString@@QAEXPBDPAD@Z
#ifndef FABLE_PASS11_CSTRING_H
#define FABLE_PASS11_CSTRING_H
#include <stddef.h>
#include <stdarg.h>
// Retail opt-in (Codex pass 11): no pre-existing engine/CString.h was supplied.
// Layout: real libpk MFC-derived CString has its sole character pointer at +0;
// CStringData is the preceding three native long words at -12, -8 and -4.
// Native SetAt/Replace/FormatV and original MFC CString declarations corroborate.
typedef void (__fastcall* PFNASSERTHOOK)(const char* message);
extern PFNASSERTHOOK g_pfnAssertHook;
struct CDebugBreak { CDebugBreak(); };
#define IsValidString(lpsz) ((lpsz) != NULL)
#define LIB_ASSERT(cond) if (!(cond)) { if (g_pfnAssertHook) g_pfnAssertHook(#cond "\n"); CDebugBreak(); }
struct CStringData {
    long nRefs;
    long nDataLength;
    long nAllocLength;
    char* data() { return (char*)(this + 1); }
};
class CString {
public:
    char* m_pchData;
    CStringData* GetData() const {
        LIB_ASSERT(m_pchData != NULL)
        return ((CStringData*)m_pchData)-1;
    }
    int GetAllocLength() const { return GetData()->nAllocLength; }
    char* GetBuffer(int nMinBufLength);
    void ReleaseBuffer(int nNewLength = -1);
    void FormatV(const char* lpszFormat, va_list argList);
};
#endif

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
extern "C" int puts(const char*);
extern "C" void exit(int);
static unsigned test_case;
extern "C" void* malloc(size_t);
extern "C" void free(void*);
static unsigned test_assertions,test_breaks,test_allocations,test_releases;
static int test_capacity;
static void check_test(bool value){if(!value){char message[64];sprintf(message,"CString fixture failure case %u",test_case);puts(message);exit(1);}}
static int same_text(const char* a,const char* b){while(*a && *a==*b){++a;++b;}return *a==*b;}
static void __fastcall test_assert_hook(const char*){++test_assertions;}
PFNASSERTHOOK g_pfnAssertHook=test_assert_hook;
CDebugBreak::CDebugBreak(){++test_breaks;}
extern "C" int __stdcall lstrlenA(const char* text){const char* p=text;while(*p)++p;return (int)(p-text);}
#ifndef _MSC_VER
extern "C" unsigned char* _mbsinc(const unsigned char* p){return (unsigned char*)p+1;}
extern "C" int _mbclen(const unsigned char*){return 1;}
extern "C" int _ismbcdigit(unsigned int c){return c>='0' && c<='9';}
extern "C" int _mbsnbcmp(const unsigned char* a,const unsigned char* b,size_t n){
    while(n--){if(*a!=*b)return (int)*a-(int)*b;if(!*a)return 0;++a;++b;}return 0;
}
extern "C" size_t wcslen(const unsigned short* s){size_t n=0;while(s[n])++n;return n;}
#endif
char* CString::GetBuffer(int length){
    check_test(length>=0 && length<10000);++test_allocations;test_capacity=length;
    CStringData* p=(CStringData*)malloc(sizeof(CStringData)+length+17);
    check_test(p!=0);p->nRefs=1;p->nDataLength=0;p->nAllocLength=length;m_pchData=p->data();
    for(int i=0;i<length+17;++i)m_pchData[i]=(char)0x6d;
    return m_pchData;
}
void CString::ReleaseBuffer(int length){
    ++test_releases;check_test(length==-1);GetData()->nDataLength=lstrlenA(m_pchData);
    for(int i=1;i<=16;++i)check_test((unsigned char)m_pchData[test_capacity+i]==0x6d);
}
static void test_format(const char* expected,int expected_capacity,const char* format,...){
    ++test_case;CString text;text.m_pchData=0;test_allocations=test_releases=test_assertions=test_breaks=0;
    va_list args;va_start(args,format);text.FormatV(format,args);va_end(args);
    check_test(same_text(expected,text.m_pchData));
    check_test(test_allocations==1 && test_releases==1 && !test_assertions && !test_breaks);
    if(expected_capacity>=0)check_test(test_capacity==expected_capacity);
    check_test(text.GetData()->nDataLength==(long)lstrlenA(expected));free(text.GetData());
}
int main(){
    test_format("",0,"");test_format("abc",3,"abc");test_format("%",1,"%%");
    test_format("x42y",34,"x%dy",42);test_format("-17",32,"%i",-17);
    test_format("4294967295",32,"%u",~0U);test_format("ff FF 377",98,"%x %X %o",255,255,255);
    test_format("0x2a",34,"%#x",42);test_format("+0042",32,"%+05d",42);
    test_format(" 00042",32,"%6.5d",42);test_format("   042",32,"%*.*d",6,3,42);
    test_format("a",1,"%s","a");test_format("",1,"%s","");test_format("(null)",6,"%s",(const char*)0);
    test_format("     abc",8,"%8.3s","abcdefgh");test_format("abc     ",8,"%-8.3s","abcdefgh");
    test_format("  ab",4,"%*.*s",4,2,"abcde");test_format("",4,"%.0s","abcd");
    test_format("a:b",5,"%c:%c",'a','b');test_format("1.50",-1,"%.2f",1.5);
    test_format("  1.50",-1,"%6.2f",1.5);test_format("-0.250000",-1,"%f",-0.25);
    test_format("1.25",128,"%g",1.25);
#ifdef _MSC_VER
    test_format("1.50e+000",128,"%.2e",1.5);
#else
    test_format("1.50e+00",128,"%.2e",1.5);
#endif
    int written=-1;test_format("abCD",4,"ab%nCD",&written);check_test(written==2);
#ifdef _MSC_VER
    const unsigned short wide[4]={'a','b','c',0};
    test_format("abc",3,"%S",wide);test_format("abc",3,"%ls",wide);
    test_format("x",2,"%C",(int)'x');test_format("abc",3,"%hs","abc");
    test_format("1234567890123",32,"%I64d",(__int64)1234567890123);
#endif
    for(int width=1;width<=32;++width){
        char expected[64];sprintf(expected,"%*.*s",width,3,"abcdefgh");
        test_format(expected,width>3?width:3,"%*.*s",width,3,"abcdefgh");
        sprintf(expected,"%0*d",width,23);test_format(expected,width>32?width:32,"%0*d",width,23);
    }
    puts("00c0d1b0_TEST PASS");return 0;
}