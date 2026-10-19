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
// CConsole::AddInput(CConsoleInputBase*) -- register input under a lowercase copy of its name, shared through a counted pointer.
// Retail 0x009EC5E0, 296 bytes. CCharString copy0099EC30, ToLower0099ED10, destructor0099EAE0;
// CConsole input map insert009EFB80, typed DeleteData009EDC50. Native RegisteredConsoleInputs+40 vs donor+48.
// Field-free native API adapter derives the actual shared string class: storage and destructor stay in CCharString.
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

struct CRetailString : CCharString {
    CRetailString(); // 0x0099E4B0 native default
    CRetailString(const CRetailString&); // 0x0099EC30 native copy
    CRetailString ToLower() const; // 0x0099ED10
};
struct CConsoleInputBase {
    virtual ~CConsoleInputBase(); // real slot0 deleting destructor consumed by DeleteData
    CRetailString Name; // +4 native and donor layout
};
void FABLE_FASTCALL DeleteInputData(CConsoleInputBase*); // 0x009EDC50
struct SInputCountBlock {
    long refs;
    void (FABLE_FASTCALL* destroy)(CConsoleInputBase*);
    CConsoleInputBase* object;
    SInputCountBlock(CConsoleInputBase* p) : refs(1), destroy(DeleteInputData), object(p) { }
};
struct CInputPointer {
    CConsoleInputBase* object;
    SInputCountBlock* count;
    explicit CInputPointer(CConsoleInputBase* p) : object(p), count(p ? new SInputCountBlock(p) : 0) { }
    CInputPointer(const CInputPointer& other) : object(other.object), count(other.count) { if(count) ++count->refs; }
    ~CInputPointer() {
        if(count) { --count->refs; if(!count->refs) { count->destroy(count->object); delete count; } }
        object=0;count=0;
    }
};
struct CInputPair {
    CRetailString name;
    CInputPointer input;
    CInputPair(const CRetailString& n,const CInputPointer& p):name(n),input(p) { }
};
struct CInputValue {
    const CRetailString name;
    CInputPointer input;
    CInputValue(const CInputPair& p):name(p.name),input(p.input) { }
};
struct CInputInsertResult { void* iterator; bool inserted; CInputInsertResult(); };
struct CInputMap {
    CInputInsertResult Insert(const CInputValue&); // 0x009EFB80
};
struct CConsole {
    unsigned char _nativeBeforeRegisteredInputs[0x40];
    CInputMap RegisteredConsoleInputs;
    void AddInput(CConsoleInputBase*);
};
void CConsole::AddInput(CConsoleInputBase* input)
{
    CInputPointer pointer(input);
    RegisteredConsoleInputs.Insert(CInputPair(input->Name.ToLower(),pointer));
}

static int g_stringInstances,g_dataLive,g_inputDestroyed,g_deleteCalls,g_insertCalls;
static int g_entryRefs[4];
CCharString::CCharString():storage_(0) { ++g_stringInstances; }
CCharString::~CCharString() {
    if(storage_ && --storage_->owners==0) { delete[] storage_->text;delete storage_;--g_dataLive; }
    --g_stringInstances;
}
CCharString& CCharString::operator=(const CCharString& other) {
    if(this==&other) return *this;
    CCharStringData* next=other.storage_;if(next) ++next->owners;
    if(storage_ && --storage_->owners==0) {delete[] storage_->text;delete storage_;--g_dataLive;}
    storage_=next;return *this;
}
const CCharString& CCharString::operator=(const char* text) {
    CCharStringData* next=new CCharStringData;
    next->text=new char[std::strlen(text)+1];std::strcpy(next->text,text);
    next->unknown04=next->unknown08=0;next->flags0C=0;next->owners=1;++g_dataLive;
    if(storage_ && --storage_->owners==0) {delete[] storage_->text;delete storage_;--g_dataLive;}
    storage_=next;return *this;
}
CCharString::operator const char*() const {return storage_ ? storage_->text : "";}
CRetailString::CRetailString():CCharString() { }
CRetailString::CRetailString(const CRetailString& other):CCharString() {CCharString::operator=(other);}
CRetailString CRetailString::ToLower() const {
    const char* source=static_cast<const char*>(*this);
    char* text=new char[std::strlen(source)+1];std::strcpy(text,source);
    for(char* p=text;*p;++p) *p=(char)std::tolower((unsigned char)*p);
    CRetailString result;result.CCharString::operator=(text);delete[] text;return result;
}
CConsoleInputBase::~CConsoleInputBase() { ++g_inputDestroyed; }
void FABLE_FASTCALL DeleteInputData(CConsoleInputBase* p) { ++g_deleteCalls;if(p) delete p; }
CInputInsertResult::CInputInsertResult():iterator(0),inserted(false) { }
struct MapSlot { CInputMap* map;CInputValue* value; };
static MapSlot g_entries[4];static int g_size;
CInputInsertResult CInputMap::Insert(const CInputValue& value) {
    CInputInsertResult result;
    g_entryRefs[g_insertCalls++]=value.input.count ? value.input.count->refs : 0;
    for(int i=0;i<g_size;++i) if(g_entries[i].map==this && !std::strcmp(g_entries[i].value->name,value.name)) {result.iterator=g_entries[i].value;return result;}
    g_entries[g_size].map=this;g_entries[g_size].value=new CInputValue(value);
    result.iterator=g_entries[g_size++].value;result.inserted=true;return result;
}
static int Fail(const char* why) {std::printf("009ec5e0_TEST FAIL %s\n",why);return 1;}
int main() {
    CConsole console;
    CConsoleInputBase* first=new CConsoleInputBase;first->Name.CCharString::operator=("HeLP");
    console.AddInput(first);
    if(g_size!=1 || g_entryRefs[0]!=3 || g_entries[0].value->input.count->refs!=1 || g_entries[0].value->input.object!=first) return Fail("insert/count ownership");
    if(std::strcmp(g_entries[0].value->name,"help") || std::strcmp(first->Name,"HeLP") || (const char*)g_entries[0].value->name==(const char*)first->Name) return Fail("lowercase independent key");
    if(g_stringInstances!=2 || g_dataLive!=2 || g_inputDestroyed) return Fail("temporary cleanup");
    CConsoleInputBase* duplicate=new CConsoleInputBase;duplicate->Name.CCharString::operator=("HELP");console.AddInput(duplicate);
    if(g_size!=1 || g_entryRefs[1]!=3 || g_entries[0].value->input.object!=first || g_inputDestroyed!=1 || g_deleteCalls!=1 || g_stringInstances!=2 || g_dataLive!=2) return Fail("duplicate rejects and releases new input");
    CConsoleInputBase* second=new CConsoleInputBase;second->Name.CCharString::operator=("Status");console.AddInput(second);
    if(g_size!=2 || g_entryRefs[2]!=3 || std::strcmp(g_entries[1].value->name,"status") || g_entries[1].value->input.count->refs!=1) return Fail("distinct insertion");
    for(int i=0;i<g_size;++i) delete g_entries[i].value;g_size=0;
    if(g_inputDestroyed!=3 || g_deleteCalls!=3 || g_stringInstances || g_dataLive) return Fail("complete teardown");
    std::printf("009ec5e0_TEST PASS\n");return 0;
}