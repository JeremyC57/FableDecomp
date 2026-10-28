#include <stdio.h>
#include <stdlib.h>
#define FABLE_FASTCALL
typedef unsigned int fable_u32;typedef int fable_i32;typedef unsigned char fable_u8;
static int constructed,destroyed,bad;static void CheckLifetime();struct RegisterCheck{RegisterCheck(){atexit(CheckLifetime);}};static RegisterCheck registerCheck;
#pragma once



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

CWideString g_UnprovenWideString_0143E908;
fable_i32 g_CWideStringInstanceCount_013BCA20;CWideString::CWideString():storage_(0){if(this!=&g_UnprovenWideString_0143E908)bad=1;++constructed;++g_CWideStringInstanceCount_013BCA20;}CWideString::~CWideString(){if(this!=&g_UnprovenWideString_0143E908)bad=1;++destroyed;--g_CWideStringInstanceCount_013BCA20;}static void CheckLifetime(){puts(!bad&&constructed==1&&destroyed==1&&!g_CWideStringInstanceCount_013BCA20?"01224c83_TEST PASS":"01224c83_TEST FAIL");}int main(){return constructed==1&&!destroyed&&!bad&&g_CWideStringInstanceCount_013BCA20==1?0:1;}