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
// CFH_Prisoner::GetEndSpeech(CCharString&). Actual string storage/lifetime header.
// Retail quest fields88/184, owner14. No matching quest/handler header available.
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

// No-data extension recovers missing genuine string methods. Copy constructor
// is PDB egor_pdb_names.tsv30802; actual inherited storage and destructor retained.
struct CPrisonerSpeechString:CCharString {
 CPrisonerSpeechString(const char*,long);
 CPrisonerSpeechString(const CPrisonerSpeechString&);
 const CCharString& Append(const char*); //99F100
};
CPrisonerSpeechString __fastcall PrisonerNumberString(int); //99F830 resultECX, numberEDX
CPrisonerSpeechString __fastcall operator+(const char*,const CPrisonerSpeechString&); //99F690
struct CPrisonerQuestState {unsigned char Prefix[0x88];int FreedCount;unsigned char BeforeOutcome[0xf8];int Outcome;};
struct CFH_Prisoner {unsigned char Prefix[0x14];CPrisonerQuestState* Quest;bool IsActiveThreadTerminating()const;void GetEndSpeech(CPrisonerSpeechString&)const;}; //F35B30
void CFH_Prisoner::GetEndSpeech(CPrisonerSpeechString& out)const{
 static_cast<CCharString&>(out)="TEXT_QST_B01_PROPHET"+PrisonerNumberString(Quest->FreedCount+1);
 if(Quest->FreedCount==0){if(IsActiveThreadTerminating())return;out.Append("_FREED");return;}
 if(IsActiveThreadTerminating())return;
 if(Quest->Outcome==1){if(IsActiveThreadTerminating())return;out.Append("_FREED_ASEXPECTED");return;}
 if(Quest->Outcome==2){if(IsActiveThreadTerminating())return;out.Append("_FREED_DEFAULT");return;}
 if(Quest->Outcome==3){if(IsActiveThreadTerminating())return;out.Append("_FREED_SURPRISE");return;}
 IsActiveThreadTerminating();
}



#include <assert.h>
#include <stdio.h>
#include <string.h>
static int liveData,liveObjects,terminationChecks,terminateAt;
CCharString::CCharString():storage_(0){++liveObjects;}
CCharString::CCharString(const char* s,long n):storage_(0){++liveObjects;assert(n==-1);operator=(s);}
void CCharString::UnassignString(){if(storage_&&!--storage_->owners){delete[] storage_->text;delete storage_;--liveData;}storage_=0;}
CCharString::~CCharString(){UnassignString();--liveObjects;}
const CCharString& CCharString::operator=(const char* s){UnassignString();storage_=new CCharStringData;storage_->owners=1;storage_->unknown04=storage_->unknown08=0;storage_->flags0C=0;storage_->text=new char[strlen(s)+1];strcpy(storage_->text,s);++liveData;return *this;}
CCharString& CCharString::operator=(const CCharString& s){if(storage_!=s.storage_){UnassignString();storage_=s.storage_;if(storage_)++storage_->owners;}return *this;}
CCharString::operator const char*()const{return storage_?storage_->text:"";}
CPrisonerSpeechString::CPrisonerSpeechString(const char*s,long n):CCharString(s,n){}
CPrisonerSpeechString::CPrisonerSpeechString(const CPrisonerSpeechString&s):CCharString(){CCharString::operator=(s);}
const CCharString& CPrisonerSpeechString::Append(const char* suffix){char b[200];strcpy(b,*this);strcat(b,suffix);return CCharString::operator=(b);}
CPrisonerSpeechString __fastcall PrisonerNumberString(int n){char b[32];sprintf(b,"%d",n);return CPrisonerSpeechString(b,-1);}
CPrisonerSpeechString __fastcall operator+(const char* prefix,const CPrisonerSpeechString&s){char b[200];strcpy(b,prefix);strcat(b,s);return CPrisonerSpeechString(b,-1);}
bool CFH_Prisoner::IsActiveThreadTerminating()const{++terminationChecks;assert(liveObjects==1&&liveData==1);return terminationChecks==terminateAt;}
int main(){for(int freed=0;freed<3;++freed)for(int outcome=0;outcome<5;++outcome)for(int stop=0;stop<3;++stop){
 CPrisonerQuestState quest;quest.FreedCount=freed;quest.Outcome=outcome;CFH_Prisoner p;p.Quest=&quest;terminationChecks=0;terminateAt=stop;
 {CPrisonerSpeechString out("old",-1);p.GetEndSpeech(out);char expected[200];sprintf(expected,"TEXT_QST_B01_PROPHET%d",freed+1);
  if(!freed){if(stop!=1)strcat(expected,"_FREED");assert(terminationChecks==1);}
  else {assert(terminationChecks==(stop==1?1:2));if(stop!=1&&stop!=2&&outcome>=1&&outcome<=3)strcat(expected,outcome==1?"_FREED_ASEXPECTED":outcome==2?"_FREED_DEFAULT":"_FREED_SURPRISE");}
  assert(strcmp(out,expected)==0);assert(liveObjects==1&&liveData==1);
 }assert(!liveObjects&&!liveData);
 }puts("00d3abe0 all suffix branches, thread termination and string ownership PASS");return 0;}
#ifdef _MSC_VER
typedef char String4[sizeof(CPrisonerSpeechString)==4?1:-1];
typedef char Quest14[offsetof(CFH_Prisoner,Quest)==0x14?1:-1];
typedef char Outcome184[offsetof(CPrisonerQuestState,Outcome)==0x184?1:-1];
#endif