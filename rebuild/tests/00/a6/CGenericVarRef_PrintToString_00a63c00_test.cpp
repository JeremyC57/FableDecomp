#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <new>
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __cdecl
#define __declspec(x)
#define __int64 long long
#endif
// CGenericVarRef::PrintToString -- format a referenced integer, float, bool, or string
// Leaf: PrintToString
// Retail00A63C00,180bytes including embedded fiveentry jump table.
// Actual string header lacks the native public copy ctor; a zero-state derived API façade
// exposes that ctor without replicating string layout. Its implicit destructor calls the actual base string destructor.
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


class CWideStringData
{ public:
    wchar_t* text;
    fable_u32 unknown04;
    fable_u32 unknown08;
    fable_i32 owners;
};

#pragma pack(push, 1)
class CCharStringData
{ public:
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
    const CWideString& operator=(const CWideString& other);
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
    const CCharString& operator=(const CCharString& other);
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

struct NativeStringCopy:CCharString{NativeStringCopy(const CCharString&);}; //0099EC30 actual CCharString copy ctor
void __cdecl Format(CCharString*,const char*,...); //0099F1F0 CCharString::Format (varargs cdecl this on stack)
struct CGenericVarRef{long Type;void*PData;void PrintToString(CCharString&)const;};
void CGenericVarRef::PrintToString(CCharString&out)const{
 switch(Type){
 case 3:Format(&out,"%f",*static_cast<const float*>(PData));break;
 case 5:{NativeStringCopy copy(*static_cast<const CCharString*>(PData));out=static_cast<const CCharString&>(copy);break;}
 case 4:if(*static_cast<const bool*>(PData))out="TRUE";else out="FALSE";break;
 case 1:Format(&out,"%d",*static_cast<const long*>(PData));break;
 }
}



#include <stdio.h>
#include <string.h>
#include <stdarg.h>
static double LastFloat;static int Live,Copies,Assigns,Formats;static bool Bad;static char Formatted[256];
CCharString::CCharString():storage_(0){++Live;}
CCharString::~CCharString(){if(storage_&&!--storage_->owners){delete[] storage_->text;delete storage_;}--Live;}
CCharString::operator const char*()const{return storage_?storage_->text:"";}
const CCharString& CCharString::operator=(const char*s){if(storage_&&!--storage_->owners){delete[] storage_->text;delete storage_;}storage_=0;if(s&&*s){storage_=new CCharStringData;storage_->text=new char[strlen(s)+1];strcpy(storage_->text,s);storage_->owners=1;}return *this;}
const CCharString& CCharString::operator=(const CCharString&s){++Assigns;if(s.storage_)++s.storage_->owners;if(storage_&&!--storage_->owners){delete[] storage_->text;delete storage_;}storage_=s.storage_;return *this;}
// Genuine native copy stub: implicit base bit copy, then shared-owner retain; no invented engine ctor.
NativeStringCopy::NativeStringCopy(const CCharString&s):CCharString(s){++Live;++Copies;CCharStringData*d=*reinterpret_cast<CCharStringData**>(this);if(d)++d->owners;}
void __cdecl Format(CCharString*out,const char*fmt,...){++Formats;va_list ap;va_start(ap,fmt);if(!strcmp(fmt,"%d")){long n=va_arg(ap,long);sprintf(Formatted,"%ld",n);}else if(!strcmp(fmt,"%f")){double d=va_arg(ap,double);LastFloat=d;sprintf(Formatted,"%f",d);}else Bad=true;va_end(ap);*out=Formatted;}
#define CHECK(x) do{if(!(x)){printf("00a63c00_TEST FAIL line %d\n",__LINE__);return 1;}}while(0)
int main(){CCharString out,text;out="sentinel";text="sample";CGenericVarRef ref;long integer=-2147483647L-1;ref.Type=1;ref.PData=&integer;ref.PrintToString(out);CHECK(!Bad&&!strcmp(static_cast<const char*>(out),"-2147483648")&&Formats==1&&Copies==0&&Live==2);integer=2147483647L;ref.PrintToString(out);CHECK(!strcmp(static_cast<const char*>(out),"2147483647"));
float value=1.25f;ref.Type=3;ref.PData=&value;ref.PrintToString(out);CHECK(!Bad&&!strcmp(static_cast<const char*>(out),"1.250000"));unsigned int negzero=0x80000000;memcpy(&value,&negzero,4);ref.PrintToString(out);CHECK(LastFloat==0.0&&(reinterpret_cast<const unsigned char*>(&LastFloat)[7]&128));
bool flag=false;ref.Type=4;ref.PData=&flag;ref.PrintToString(out);CHECK(!strcmp(static_cast<const char*>(out),"FALSE"));flag=true;ref.PrintToString(out);CHECK(!strcmp(static_cast<const char*>(out),"TRUE"));
ref.Type=5;ref.PData=&text;ref.PrintToString(out);CHECK(!strcmp(static_cast<const char*>(out),"sample")&&Copies==1&&Assigns==1&&Live==2);CCharStringData*d=*reinterpret_cast<CCharStringData**>(&text);CHECK(d->owners==2);
ref.PData=&out;ref.PrintToString(out);CHECK(!strcmp(static_cast<const char*>(out),"sample")&&Copies==2&&Assigns==2&&Live==2&&d->owners==2);
text="";ref.PData=&text;ref.PrintToString(out);CHECK(!strcmp(static_cast<const char*>(out),"")&&Copies==3&&Live==2);
out="unchanged";long invalid[]={0,2,6,-1,2147483647L};ref.PData=0;for(int i=0;i<5;++i){ref.Type=invalid[i];ref.PrintToString(out);CHECK(!strcmp(static_cast<const char*>(out),"unchanged"));}
CHECK(!Bad&&Live==2);printf("00a63c00_TEST PASS\n");return 0;}