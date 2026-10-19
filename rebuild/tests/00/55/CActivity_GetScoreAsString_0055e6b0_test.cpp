// Host-only declaration adapter mirrors fable_string.h; target compiles its real shared header.
#ifndef _MSC_VER
#include <stddef.h>
#define FABLE_WIDE_STRING_HOST_TYPES
#define FABLE_FASTCALL
#define __cdecl
typedef unsigned int fable_u32;
typedef int fable_i32;
typedef unsigned char fable_u8;



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

#endif
// CActivity::GetScoreAsString(float) const -- format the score to its configured precision and append the localized unit.
// Retail: 0x0055E6B0, 263 bytes. Leaf: GetScoreAsString
// No CActivity header exists. PDB: Name+0, Description+4, UnitOfMeasurement+8,
// NumDecPlacesToShow+c and LowerIsBetter+d. Shared fable_string.h supplies strings;
// the method-only wide string view adds FindCharPos and SetLength omitted there.
// Calls: string ctor0099AED0/copy0099B720/dtor0099B510, assignment0099B7D0,
// append0099B940/0099B8D0; formatter0099BA70; GFFloatToWideString00486C40;
// FindCharPos0099B070, SetLength0099BC50; GetTextBySymbol009C95E0.
#ifndef FABLE_WIDE_STRING_HOST_TYPES
#include "fable_string.h"
#endif
void __cdecl FormatWideString(CWideString*, const wchar_t*, ...); //0099BA70
inline CWideString IntToWideString(long value) {
    CWideString text;
    FormatWideString(&text,L"%d",value); //0122DE44
    return text;
}
CWideString FABLE_FASTCALL GFFloatToWideString(double); //00486C40
struct CWideStringOperations : CWideString {
    long FindCharPos(wchar_t) const; //0099B070
    void SetLength(long); //0099BC50
};
namespace NGameText { struct CDataBank { CWideString GetTextBySymbol(const CCharString&); }; }
struct CMainGameComponent { unsigned char beforeTextBank[0x14]; NGameText::CDataBank* PTextBank; NGameText::CDataBank* GetTextBank() const { return PTextBank; } };
extern CMainGameComponent* g_pMainGameComponent; //013B86A0
struct CActivity {
    CCharString Name,Description,UnitOfMeasurement;
    unsigned char NumDecPlacesToShow;
    bool LowerIsBetter;
    CWideString GetScoreAsString(float) const;
};
CWideString CActivity::GetScoreAsString(float score) const
{
    CWideString result;
    if (!NumDecPlacesToShow) result=IntToWideString((long)score);
    else {
        CWideString value=GFFloatToWideString(score);
        CWideStringOperations& operations=reinterpret_cast<CWideStringOperations&>(value);
        operations.SetLength(operations.FindCharPos(L'.')+1+NumDecPlacesToShow);
        result=value;
    }
    result+=L" "; //0122D820
    result+=g_pMainGameComponent->GetTextBank()->GetTextBySymbol(UnitOfMeasurement);
    return result;
}

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
static int wideLive,charLive,invalid,formatCalls,floatCalls,findCalls,lengthCalls,textCalls;
static long formattedInteger,lastLength;
static double floatValue;
static const wchar_t* floatText;
static NGameText::CDataBank* expectedBank;
static const CCharString* expectedUnit;
static const wchar_t* unitText;
static long wlength(const wchar_t* s) { long n=0; while(s[n]) ++n; return n; }
static bool wequal(const wchar_t* a,const wchar_t* b) { while(*a && *a==*b) {++a;++b;} return *a==*b; }
static CWideStringData* wideData(const wchar_t* s) {
    CWideStringData* d=new CWideStringData; long n=wlength(s);
    d->text=new wchar_t[n+1]; for(long i=0;i<=n;++i)d->text[i]=s[i]; d->owners=1; return d;
}
CWideString::CWideString() : storage_(wideData(L"")) { ++wideLive; }
CWideString::CWideString(const wchar_t* s) : storage_(wideData(s)) { ++wideLive; }
CWideString::CWideString(const CWideString& o) : storage_(wideData((const wchar_t*)o)) { ++wideLive; }
CWideString::~CWideString() { delete[] storage_->text;delete storage_;--wideLive; }
CWideString::operator const wchar_t*() const { return storage_->text; }
CWideString& CWideString::operator=(const CWideString& o) { if(this!=&o) *this=(const wchar_t*)o;return *this; }
const CWideString& CWideString::operator=(const wchar_t* s) { CWideStringData* next=wideData(s);delete[] storage_->text;delete storage_;storage_=next;return *this; }
const CWideString& CWideString::operator+=(const wchar_t* s) {
    wchar_t buffer[1024]; long n=wlength(storage_->text), m=wlength(s);
    if(n+m>=1024) { ++invalid;return *this; }
    for(long i=0;i<n;++i)buffer[i]=storage_->text[i];for(long j=0;j<=m;++j)buffer[n+j]=s[j];
    *this=buffer; return *this;
}
const CWideString& CWideString::operator+=(const CWideString& o) { return *this+=(const wchar_t*)o; }
CCharString::CCharString() { storage_=new CCharStringData;storage_->text=(char*)""; ++charLive; }
CCharString::CCharString(const char* s,long n) { if(n!=-1)++invalid;storage_=new CCharStringData;storage_->text=(char*)s;++charLive; }
CCharString::~CCharString() { delete storage_;--charLive; }
CCharString::operator const char*() const { return storage_->text; }
void __cdecl FormatWideString(CWideString* result,const wchar_t* fmt,...) {
    ++formatCalls;if(!wequal(fmt,L"%d"))++invalid;
    va_list ap;va_start(ap,fmt);formattedInteger=va_arg(ap,long);va_end(ap);
    char narrow[40];sprintf(narrow,"%ld",formattedInteger);wchar_t wide[40];long i=0;
    do {wide[i]=(unsigned char)narrow[i];}while(narrow[i++]); *result=wide;
}
CWideString FABLE_FASTCALL GFFloatToWideString(double value) { ++floatCalls;floatValue=value;return CWideString(floatText); }
long CWideStringOperations::FindCharPos(wchar_t c) const {
    ++findCalls;if(c!=L'.')++invalid;const wchar_t* s=*this;for(long i=0;s[i];++i)if(s[i]==c)return i;return -1;
}
void CWideStringOperations::SetLength(long n) {
    ++lengthCalls;lastLength=n;wchar_t buffer[1024];const wchar_t* s=*this;long old=wlength(s);
    if(n<0 || n>=1024) {++invalid;return;}
    for(long i=0;i<n;++i)buffer[i]=i<old?s[i]:0;buffer[n]=0;CWideString::operator=(buffer);
}
CWideString NGameText::CDataBank::GetTextBySymbol(const CCharString& unit) {
    ++textCalls;if(this!=expectedBank || &unit!=expectedUnit)++invalid;return CWideString(unitText);
}
CMainGameComponent* g_pMainGameComponent;
static int run(CActivity& a,float value,unsigned char precision,const wchar_t* formatted,const wchar_t* expected,long expectedLength) {
    a.NumDecPlacesToShow=precision;formatCalls=floatCalls=findCalls=lengthCalls=textCalls=invalid=0;
    floatText=formatted;int baseline=wideLive;
    int ok;
    { CWideString result=a.GetScoreAsString(value);
      ok=wequal((const wchar_t*)result,expected) && invalid==0 && textCalls==1 && wideLive==baseline+1;
      if(precision) ok=ok && formatCalls==0 && floatCalls==1 && findCalls==1 && lengthCalls==1 && lastLength==expectedLength && floatValue==(double)value;
      else ok=ok && formatCalls==1 && formattedInteger==(long)value && floatCalls==0 && findCalls==0 && lengthCalls==0;
    }
    return ok && wideLive==baseline;
}
int main() {
    int ok=1;
    { CActivity a; NGameText::CDataBank bank; CMainGameComponent component;
      component.PTextBank=&bank;g_pMainGameComponent=&component;expectedBank=&bank;expectedUnit=&a.UnitOfMeasurement;
      unitText=L"points";
      ok=run(a,12.875f,0,L"",L"12 points",0) && run(a,-12.875f,0,L"",L"-12 points",0)
          && run(a,0.0f,0,L"",L"0 points",0) && run(a,12.345f,2,L"12.345000",L"12.34 points",5)
          && run(a,-1.25f,1,L"-1.250000",L"-1.2 points",4) && run(a,42.0f,2,L"42",L"42 points",2);
      unitText=L"";ok=ok && run(a,7.0f,0,L"",L"7 ",0);
      unitText=L"m/s";ok=ok && run(a,1.125f,3,L"1.125000",L"1.125 m/s",5);
    }
    ok=ok && wideLive==0 && charLive==0;
    printf("0055e6b0_TEST %s\n",ok?"PASS":"FAIL");return ok?0:1;
}