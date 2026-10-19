#include <stdio.h>
#include <string.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <wchar.h>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __int64 long long
#endif
// CCRCSymbolMap::Error(const CWideString&,long,const char*,...) -- build and release a compiler-style diagnostic
// Retail0x00A381D0,263 bytes. Native member varargs ABI puts this on stack. Buffers are line[12] and message[256].
// Literal0x0129C14C="(%d)",0x0129C138=" : error C2065 : ",0x0122D70E="". /GS cookie is present.
// Flags: /GS
#pragma once

#include <stddef.h>

#pragma once

// ABI primitives for reconstructed FableTLC modules. Keep this header valid
// under VC7.1; modern-only language/library features belong outside Tier 0.
#if defined(_MSC_VER)
#  define FABLE_CDECL __cdecl
#  define FABLE_STDCALL __stdcall
#  define FABLE_FASTCALL __fastcall
#  define FABLE_THISCALL __fastcall
#else
#  define FABLE_CDECL
#  define FABLE_STDCALL
#  define FABLE_FASTCALL
#  define FABLE_THISCALL
#endif

typedef signed char fable_i8;
typedef unsigned char fable_u8;
typedef signed short fable_i16;
typedef unsigned short fable_u16;
typedef signed int fable_i32;
typedef unsigned int fable_u32;

typedef char fable_require_i8[sizeof(fable_i8) == 1 ? 1 : -1];
typedef char fable_require_i16[sizeof(fable_i16) == 2 ? 1 : -1];
typedef char fable_require_i32[sizeof(fable_i32) == 4 ? 1 : -1];
#ifdef _MSC_VER
typedef char fable_require_ptr32[sizeof(void*) == 4 ? 1 : -1];
#endif

#define FABLE_JOIN_INNER(a, b) a##b
#define FABLE_JOIN(a, b) FABLE_JOIN_INNER(a, b)
#ifndef _MSC_VER
#define FABLE_STATIC_ASSERT(expression)
#else
#define FABLE_STATIC_ASSERT(expression) \
    typedef char FABLE_JOIN(fable_abi_assert_, __LINE__)[(expression) ? 1 : -1]
#endif


struct CWideStringData
{
    wchar_t* text;
    fable_u32 unknown04;
    fable_u32 unknown08;
    fable_i32 owners;
};

#pragma pack(push, 1)
struct CCharStringData
{
    char* text;
    fable_u32 unknown04;
    fable_u32 unknown08;
    fable_u8 flags0C;
    fable_i32 owners;
};
#pragma pack(pop)

class CCharString;

class CWideString
{
public:
    CWideString();
    CWideString(const CWideString& other);
    explicit CWideString(const wchar_t* text);
    ~CWideString();
    operator const wchar_t*() const;
    long GetLength() const;
    CWideString& operator=(const CWideString& other);
    const CWideString& operator=(const wchar_t* text);
    const CWideString& operator+=(const CWideString& other);
    const CWideString& operator+=(const wchar_t* text);
    bool operator!=(const wchar_t* other) const;

    static CWideString FABLE_FASTCALL CreateFromCharString(
        const CCharString& text);

private:
    explicit CWideString(const CCharString& text);
    void UnassignString();
    CWideStringData* AllocStringData(
        const wchar_t* text,
        long length);

    CWideStringData* storage_;
};

CWideString FABLE_FASTCALL operator+(
    const CWideString& left,
    const CWideString& right);
CWideString FABLE_FASTCALL operator+(
    const CWideString& left,
    const wchar_t* right);
CWideString FABLE_FASTCALL operator+(
    const wchar_t* left,
    const CWideString& right);

class CCharString
{
public:
    CCharString();
    CCharString(const char* text, long length);
    ~CCharString();
    operator const char*() const;
    CCharString& operator=(const CCharString& other);
    const CCharString& operator=(const char* text);
    CWideString ToWideString() const;

private:
    void UnassignString();
    CCharStringData* AllocStringData(
        const char* text,
        long length);

    CCharStringData* storage_;
};

CCharString FABLE_FASTCALL operator+(
    const CCharString& left,
    const char* right);

// Retail folds equivalent narrow/wide string lifetime helpers. These counters
// remain separate because their addresses and object families are distinct.
extern fable_i32 g_CWideStringInstanceCount_013BCA20;
extern fable_i32 g_CCharStringInstanceCount_013BD800;
extern wchar_t g_FableEmptyWideString_0129A8E0[1];

FABLE_STATIC_ASSERT(sizeof(CWideStringData) == 0x10);
FABLE_STATIC_ASSERT(sizeof(CCharStringData) == 0x11);
FABLE_STATIC_ASSERT(sizeof(CWideString) == 0x04);
FABLE_STATIC_ASSERT(sizeof(CCharString) == 0x04);

#include <stdio.h>
#include <stdarg.h>
struct CRetailWideString : CWideString {
    CCharString ConvertToCharString() const; // 0x0099B2C0
};
CCharString FABLE_FASTCALL operator+(const CCharString&,const CCharString&); // 0x0099F570
// Existing operator+(CCharString,const char*) is native0x0099F600.
struct CCRCSymbolMap { void Error(const CRetailWideString& file,long line,const char* format,...); };
void CCRCSymbolMap::Error(const CRetailWideString& file,long line,const char* format,...) {
    CCharString result;
    char lineText[12]; char message[256];
    sprintf(lineText,"(%d)",line+1);
    va_list args; va_start(args,format); vsprintf(message,format,args);va_end(args);
    result=CCharString("",-1)+file.ConvertToCharString()+lineText+" : error C2065 : "+message;
}

static int liveStrings,assignments;static char observed[1024];
CCharString::CCharString():storage_(0){++liveStrings;}
CCharString::CCharString(const char* text,long n):storage_(new CCharStringData){
    ++liveStrings;if(n<0)n=(long)strlen(text);storage_->text=new char[n+1];memcpy(storage_->text,text,n);storage_->text[n]=0;
    storage_->unknown04=(fable_u32)n;storage_->unknown08=(fable_u32)n+1;storage_->flags0C=0;storage_->owners=1;
}
CCharString::~CCharString(){--liveStrings;if(storage_&&--storage_->owners==0){delete[] storage_->text;delete storage_;}}
CCharString::operator const char*()const{return storage_?storage_->text:"";}
CCharString& CCharString::operator=(const CCharString& s){
    ++assignments;strcpy(observed,(const char*)s);if(s.storage_)++s.storage_->owners;
    if(storage_&&--storage_->owners==0){delete[]storage_->text;delete storage_;}storage_=s.storage_;return *this;
}
CWideString::CWideString():storage_(0){}
CWideString::~CWideString(){if(storage_){delete[]storage_->text;delete storage_;}}
CWideString::operator const wchar_t*()const{return storage_?storage_->text:L"";}
const CWideString& CWideString::operator=(const wchar_t* s){
    if(storage_){delete[]storage_->text;delete storage_;}storage_=new CWideStringData;
    size_t n=wcslen(s);storage_->text=new wchar_t[n+1];wcscpy(storage_->text,s);storage_->owners=1;storage_->unknown04=(fable_u32)n;storage_->unknown08=(fable_u32)n+1;return *this;
}
CCharString CRetailWideString::ConvertToCharString()const{
    char text[256];const wchar_t* s=*this;size_t i=0;for(;s[i]&&i<255;++i)text[i]=(char)s[i];text[i]=0;return CCharString(text,-1);
}
CCharString FABLE_FASTCALL operator+(const CCharString& a,const CCharString& b){char text[1024];strcpy(text,(const char*)a);strcat(text,(const char*)b);return CCharString(text,-1);}
CCharString FABLE_FASTCALL operator+(const CCharString& a,const char* b){char text[1024];strcpy(text,(const char*)a);strcat(text,b);return CCharString(text,-1);}
int main(){
    CCRCSymbolMap map;CRetailWideString file;file.CWideString::operator=(L"symbols.txt");
    map.Error(file,0,"unknown '%s' (%d)","key",17);
    if(strcmp(observed,"symbols.txt(1) : error C2065 : unknown 'key' (17)")||assignments!=1||liveStrings){printf("00a381d0_TEST FAIL\n");return 1;}
    file.CWideString::operator=(L"other.map");map.Error(file,41,"bad token %c",'X');
    if(strcmp(observed,"other.map(42) : error C2065 : bad token X")||assignments!=2||liveStrings){printf("00a381d0_TEST FAIL\n");return 1;}
    file.CWideString::operator=(L"");map.Error(file,-2,"%s %x","name",42);
    if(strcmp(observed,"(-1) : error C2065 : name 2a")||assignments!=3||liveStrings)return 2;
    printf("00a381d0_TEST PASS\n");return 0;
}