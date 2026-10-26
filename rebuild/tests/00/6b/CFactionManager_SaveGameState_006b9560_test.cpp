#include <cstdio>
#include <cstdlib>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#endif
// CFactionManager::SaveGameState(CPersistContext&) const -- persist faction count and per-index faction object.
// Native006B9560 168 bytes;410BE0 transfer,99F830 integer text,99F690 concat,99E4C0 cstr,6BAED0 named faction transfer,99EAE0 dtor.
// PDB Factions vector map pair is key plus two-word counted pointer (12-byte records).
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

#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CWideStringData) == 0x10);
FABLE_STATIC_ASSERT(sizeof(CCharStringData) == 0x11);
FABLE_STATIC_ASSERT(sizeof(CWideString) == 0x04);
FABLE_STATIC_ASSERT(sizeof(CCharString) == 0x04);

#endif

CCharString __fastcall GFIntToCharString(long);
CCharString __fastcall operator+(const char*,const CCharString&);
class CFaction;
class CPersistContext { public:
 void Transfer(const char*,long&,const long&);
 void TransferFaction(const char*,CFaction&);
};
struct CountedFaction { CFaction* Data; void* Info; __forceinline CFaction& operator*() const { return *Data; } };
struct FactionEntry { unsigned long Key; CountedFaction Faction; };
class CFactionManager { public:
 FactionEntry *Start,*Finish,*EndOfStorage;
 void SaveGameState(CPersistContext&) const;
};
void CFactionManager::SaveGameState(CPersistContext& ctx) const {
 long n=Finish-Start;
 ctx.Transfer("NumberOfFactions",n,0);
 for(long i=0;i<n;++i) ctx.TransferFaction("Faction"+GFIntToCharString(i),*Start[i].Faction);
}

static int destructed,constructed; static CCharStringData* allocations[500];static int allocated;
static void CleanupStrings(){for(int i=0;i<allocated;++i){std::free(allocations[i]->text);std::free(allocations[i]);}}
static char* duptext(const char*s) { char*p=(char*)std::malloc(std::strlen(s)+1); std::strcpy(p,s); return p; }
CCharString::CCharString() { storage_=0; ++constructed; }
CCharString::CCharString(const char*s,long n) { storage_=(CCharStringData*)std::malloc(sizeof(CCharStringData)); storage_->text=duptext(s); allocations[allocated++]=storage_; ++constructed; }
CCharString::~CCharString() { ++destructed; }
CCharString::operator const char*() const {return storage_?storage_->text:"";}
CCharString& CCharString::operator=(const CCharString&s) { storage_=s.storage_;return *this; }
CCharString __fastcall GFIntToCharString(long n) { char b[40];std::sprintf(b,"%ld",n);return CCharString(b,-1); }
CCharString __fastcall operator+(const char*a,const CCharString& b) { char s[150];std::strcpy(s,a);std::strcat(s,(const char*)b);return CCharString(s,-1); }
class CFaction { public: int id; };
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(FactionEntry)==12);
FABLE_STATIC_ASSERT(offsetof(CFactionManager,Start)==0);
FABLE_STATIC_ASSERT(offsetof(CFactionManager,Finish)==4);
FABLE_STATIC_ASSERT(offsetof(CFactionManager,EndOfStorage)==8);
#endif
static int transfers, countSeen,limit=-1;static CFactionManager* mutateManager;static FactionEntry* replacement;static int baseDestroyed;
static CFaction* seen[4];static char names[4][40];
void CPersistContext::Transfer(const char*n,long&v,const long&d) {if(std::strcmp(n,"NumberOfFactions")||d!=0) std::abort();countSeen=v;if(limit>=0)v=limit;}
void CPersistContext::TransferFaction(const char*n,CFaction& f) {if(transfers>=4||destructed!=baseDestroyed+transfers*2)std::abort(); if(transfers==0 && mutateManager)mutateManager->Start=replacement;std::strcpy(names[transfers],n);seen[transfers++]=&f;}
int main() {
 CFaction f[3];FactionEntry e[3];CFactionManager m;CPersistContext ctx;
 for(int i=0;i<3;++i){e[i].Key=70+i;e[i].Faction.Data=&f[i];e[i].Faction.Info=0;}
 m.Start=e;baseDestroyed=destructed;m.Finish=e+3;m.EndOfStorage=e+3;m.SaveGameState(ctx);
 if(countSeen!=3||transfers!=3) return 1;
 for(int j=0;j<3;++j){char b[40];std::sprintf(b,"Faction%d",j);if(seen[j]!=&f[j]||std::strcmp(names[j],b))return 1;}
 transfers=0;baseDestroyed=destructed;m.Finish=e;m.SaveGameState(ctx);if(countSeen||transfers)return 1;
 transfers=0;baseDestroyed=destructed;limit=1;m.Finish=e+3;m.SaveGameState(ctx);if(transfers!=1||seen[0]!=&f[0])return 1;
 
 FactionEntry newer[3];CFaction nf[3];for(int k=0;k<3;++k){newer[k].Faction.Data=&nf[k];newer[k].Faction.Info=0;}
 transfers=0;baseDestroyed=destructed;limit=-1;m.Start=e;m.Finish=e+3;mutateManager=&m;replacement=newer;m.SaveGameState(ctx);
 if(transfers!=3||seen[0]!=&f[0]||seen[1]!=&nf[1]||seen[2]!=&nf[2]||destructed!=baseDestroyed+6)return 1;
 CleanupStrings();std::puts("006b9560_TEST PASS");return 0;
}