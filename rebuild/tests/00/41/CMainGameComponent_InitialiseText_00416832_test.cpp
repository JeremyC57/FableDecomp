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
#define __stdcall
#define __declspec(x)
#define __int64 long long
#endif

// CMainGameComponent::InitialiseText() -- report progress, allocate and open the text bank.
// Retail00416832 length170. Actual string and game-text headers included below.
// Native data-bank sizeof214 agrees hand header; virtual prefix restored (OpenReadOnly slot1).
// Actual generated CMainGameComponent header absent; native PTextBank+14 matches PDB.
// Calls99EBF0/99EAE0 string;9E9F40 progress;BFEA1A scalar new;9C85A0 ctor;
// 403E40 counted reset;415260 NLocalisation::GetFontBankName;9D56C0 OpenReadOnly.
#pragma optimize("s", on)
#pragma optimize("y", off)
#include <new>
#define CCharString CCharStringDonor
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

#undef CCharString
// Restore the native owning-copy declaration omitted by the hand header.
struct CCharString { CCharStringData* Data; CCharString(const char*,long); CCharString(const CCharString&); ~CCharString(); };
#define CDataBank CDataBankDonor
#pragma once



template<typename T>
class CIVCountedPointer
{
public:
    CIVCountedPointer()
        : data_(0)
    {
    }

    CIVCountedPointer(const CIVCountedPointer& other)
        : data_(other.data_)
    {
    }

    ~CIVCountedPointer()
    {
    }

    T* data_;
};

namespace NGameText
{
    class CGroupEntry;

    class CDataBank
    {
    public:
        CDataBank();

        CIVCountedPointer<const CGroupEntry> GetPGroupEntry(
            unsigned long index,
            bool allowPending) const;

    private:
        fable_u8 storage_[0x214];
    };
}

#undef CDataBank
struct CCPPointerInfo;
template<class T> struct CCountedPointer { T* Data;CCPPointerInfo* Info; void Reset(T*); }; //00403e40
namespace NGameText {
class CDataBank {
public:
    virtual ~CDataBank(); //slot0 009cb840
    virtual bool OpenReadOnly(const CCharString&,unsigned long); //slot1 009d56c0
    CDataBank(); //009c85a0
private:
    unsigned char BankFileAndResourceBank[0x210];
};
}
namespace NProgressDisplay { void __fastcall DisplayProgress(const CCharString&,float,bool,bool); } //009e9f40
namespace NLocalisation { CCharString __fastcall GetFontBankName(); } //00415260
struct CMainGameComponent {
    unsigned char BaseAndSampleBank[0x14];
    CCountedPointer<NGameText::CDataBank> PTextBank;
    void InitialiseText();
};
FABLE_STATIC_ASSERT(sizeof(NGameText::CDataBank)==0x214);
FABLE_STATIC_ASSERT(sizeof(NGameText::CDataBank)==sizeof(NGameText::CDataBankDonor));
FABLE_STATIC_ASSERT(offsetof(CMainGameComponent,PTextBank)==0x14);
void CMainGameComponent::InitialiseText() {
    { CCharString text("Init Text",-1); NProgressDisplay::DisplayProgress(text,-1.0f,false,false); }
    PTextBank.Reset(new NGameText::CDataBank);
    { CCharString text("Opening Text Bank",-1); NProgressDisplay::DisplayProgress(text,-1.0f,false,false); }
    NGameText::CDataBank* bank = PTextBank.Data;
    bank->OpenReadOnly(NLocalisation::GetFontBankName(),0x100);
}


static int failures=0, aliveStrings=0, banksMade=0, banksDeleted=0, resets=0, opens=0, progress=0;
static char trace[128]; static int pos=0;
static NGameText::CDataBank* openedBank=0;
static void mark(char c){trace[pos++]=c;trace[pos]=0;}
CCharString::CCharString(const char* text,long length) {
 if(length!=-1) ++failures;
 Data=(CCharStringData*)malloc(sizeof(CCharStringData));
 Data->text=(char*)text;Data->owners=1;++aliveStrings;
}
CCharString::CCharString(const CCharString& other):Data(other.Data){if(Data)++Data->owners;}
CCharString::~CCharString(){if(Data && --Data->owners==0){free(Data);--aliveStrings;}}
NGameText::CDataBank::CDataBank(){++banksMade;mark('C');}
NGameText::CDataBank::~CDataBank(){++banksDeleted;mark('D');}
bool NGameText::CDataBank::OpenReadOnly(const CCharString& s,unsigned long flags){
 ++opens;mark('O');openedBank=this;
 if(strcmp(s.Data->text,"FixtureFont") || flags!=0x100)++failures;
 return false;
}
template<class T> void CCountedPointer<T>::Reset(T* value){
 ++resets;mark('R');if(!value)++failures;
 if(Data)delete Data;Data=value;Info=0;
}
void __fastcall NProgressDisplay::DisplayProgress(const CCharString& s,float f,bool a,bool b){
 mark('P');const char* expected=(progress%2)==0?"Init Text":"Opening Text Bank";
 if(strcmp(s.Data->text,expected)||f!=-1.0f||a||b)++failures;
 ++progress;
}
CCharString __fastcall NLocalisation::GetFontBankName(){mark('F');return CCharString("FixtureFont",-1);}
int main(){
 CMainGameComponent component;component.PTextBank.Data=0;component.PTextBank.Info=0;
 component.InitialiseText();
 if(strcmp(trace,"PCRPFO")||resets!=1||banksMade!=1||banksDeleted||progress!=2||opens!=1||aliveStrings||openedBank!=component.PTextBank.Data)++failures;
 pos=0;trace[0]=0;
 component.InitialiseText();
 if(strcmp(trace,"PCRDPFO")||resets!=2||banksMade!=2||banksDeleted!=1||progress!=4||opens!=2||aliveStrings||openedBank!=component.PTextBank.Data)++failures;
 delete component.PTextBank.Data;
 if(banksDeleted!=2||aliveStrings)++failures;
 if(failures){printf("00416832_TEST FAIL %d\n",failures);return 1;}
 printf("00416832_TEST PASS\n");return 0;
}