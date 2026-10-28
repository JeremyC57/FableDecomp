#include <cstdio>
#include <cstdlib>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#endif
// CFH_Prisoner::GetEndCutscene(CCharString&) const -- form cage cutscene and append history variant.
// Native00D3AB00,213 bytes. String helpers99F830,99F690,99EFB0,99EAE0,99F100; cancellationF35B30.
// Donor CFH_Prisoner MyCageNumber+20,ParentClass+14; native matches. Parent history at184 needs donor container-size mapping audit.
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
#define FABLE_STATIC_ASSERT(expression) \


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

#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CWideStringData) == 0x10);
FABLE_STATIC_ASSERT(sizeof(CCharStringData) == 0x11);
FABLE_STATIC_ASSERT(sizeof(CWideString) == 0x04);
FABLE_STATIC_ASSERT(sizeof(CCharString) == 0x04);

#endif

struct CharStringMethods : CCharString { CCharString& Append(const char*); }; //0099F100, field-free method adapter
CCharString __fastcall GFIntToCharString(long); //0099F830
CCharString __fastcall operator+(const char*,const CCharString&); //0099F690
struct FireHeartRetailView { unsigned char OtherState[0x184]; long MissionHistory; };
struct CFH_Prisoner {
 unsigned char ScriptBase[0x14]; FireHeartRetailView* ParentClass; void* PMasterData;
 bool Chosen; long MyCageNumber;
 bool IsDone() const; //00F35B30, reads runtime info via virtualslot3 andCB7940
 void GetEndCutscene(CCharString&) const;
};
void CFH_Prisoner::GetEndCutscene(CCharString& out) const {
 out="CS_FIREHEART_KILL"+GFIntToCharString(MyCageNumber+1);
 if(MyCageNumber>0 && !IsDone()) {
  if(ParentClass->MissionHistory==1) { if(!IsDone()) static_cast<CharStringMethods&>(out).Append("S"); }
  else if(ParentClass->MissionHistory==2) { if(!IsDone()) static_cast<CharStringMethods&>(out).Append("D"); }
  else if(ParentClass->MissionHistory==3) { if(!IsDone()) static_cast<CharStringMethods&>(out).Append("X"); }
  else IsDone();
 }
}

static int destructed,constructed; static CCharStringData* allocations[500];static int allocated;
static void CleanupStrings(){for(int i=0;i<allocated;++i){std::free(allocations[i]->text);std::free(allocations[i]);}}
static char* duptext(const char*s) { char*p=(char*)std::malloc(std::strlen(s)+1); std::strcpy(p,s); return p; }
CCharString::CCharString() { storage_=0; ++constructed; }
CCharString::CCharString(const char*s,long n) { storage_=(CCharStringData*)std::malloc(sizeof(CCharStringData)); storage_->text=duptext(s); allocations[allocated++]=storage_; ++constructed; }
CCharString::~CCharString() { ++destructed; }
CCharString::operator const char*() const {return storage_?storage_->text:"";}
const CCharString& CCharString::operator=(const CCharString&s) { storage_=s.storage_;return *this; }
CCharString __fastcall GFIntToCharString(long n) { char b[40];std::sprintf(b,"%ld",n);return CCharString(b,-1); }
CCharString __fastcall operator+(const char*a,const CCharString& b) { char s[150];std::strcpy(s,a);std::strcat(s,(const char*)b);return CCharString(s,-1); }
CCharString& CharStringMethods::Append(const char*s) {char b[150];std::strcpy(b,(const char*)*this);std::strcat(b,s);CCharString t(b,-1);CCharString::operator=(t);return *this;}
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CFH_Prisoner,ParentClass)==0x14);
FABLE_STATIC_ASSERT(offsetof(CFH_Prisoner,MyCageNumber)==0x20);
FABLE_STATIC_ASSERT(offsetof(FireHeartRetailView,MissionHistory)==0x184);
#endif
static int doneCalls, cancelAt, expectedDestroyed;

bool CFH_Prisoner::IsDone() const { if(destructed!=expectedDestroyed)std::abort(); return ++doneCalls==cancelAt; }
int main() {
 FireHeartRetailView parent;CFH_Prisoner p;p.ParentClass=&parent;
 CharStringMethods output; CCharString* out=&output;
 for(int cage=0;cage<3;++cage) for(int history=0;history<5;++history) for(int cancel=0;cancel<3;++cancel) {
  p.MyCageNumber=cage;parent.MissionHistory=history;doneCalls=0;cancelAt=cancel;expectedDestroyed=destructed+2;
  p.GetEndCutscene(*out);
  char expected[100];std::sprintf(expected,"CS_FIREHEART_KILL%d",cage+1);
  if(cage>0 && cancel!=1 && cancel!=2 && history>=1 && history<=3) std::strcat(expected,history==1?"S":history==2?"D":"X");
  int calls=cage==0?0:(cancel==1?1:2);
  if(std::strcmp((const char*)*out,expected)||doneCalls!=calls||destructed!=expectedDestroyed+(cage>0 && cancel!=1 && cancel!=2 && history>=1 && history<=3 ? 1:0)) {std::puts("00d3ab00_TEST FAIL");return 1;}
 }
 CleanupStrings();std::puts("00d3ab00_TEST PASS");return 0;
}