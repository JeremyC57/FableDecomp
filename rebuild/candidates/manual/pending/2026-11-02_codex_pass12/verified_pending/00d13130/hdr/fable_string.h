#pragma once

#include <stddef.h>

#include "rebuild_abi.h"

#if defined(FABLE_RETAIL_SUBTITLE_LIFETIME) || defined(FABLE_RETAIL_MINIMAP_LIFETIME)
void* __cdecl operator new[](unsigned int);
void __cdecl operator delete[](void*);
#endif

#ifndef FABLE_RETAIL_CCHARSTRING_DATA
class CWideStringData
{ public:
    wchar_t* text;
    fable_u32 unknown04;
    fable_u32 unknown08;
    fable_i32 owners;
};

#endif

#pragma pack(push, 1)
#ifdef FABLE_RETAIL_CCHARSTRING_DATA
// Retail opt-in (Codex pass11): original Ego_r TPI fields and native
// CCharString memory accounting confirm13byte data and17byte refcount owner.
// This branch reuses the existing packed scope; default declarations stay intact.
template<class T> class CBasicString {
protected:
    T* PData;
    unsigned long StringLength;
    unsigned long DataLength : 31;
    bool UseFastExtend : 1;
#if defined(FABLE_RETAIL_BODYGUARD_IDLE) || defined(FABLE_RETAIL_FACTIONS) || defined(FABLE_RETAIL_NAVIGATION_UPDATE) || defined(FABLE_RETAIL_MINIMAP_TRAITS) || defined(FABLE_RETAIL_RELATIVE_PATH) || defined(FABLE_RETAIL_SHAPE_PERSIST) || defined(FABLE_RETAIL_SUBTITLE)
    static long __fastcall Compare(const T*,const T*);
#endif
#if defined(FABLE_RETAIL_SUBTITLE_LIFETIME) || defined(FABLE_RETAIL_MINIMAP_LIFETIME)
    void DeallocateData(){if(PData){delete[] PData;PData=0;}StringLength=0;DataLength=0;}
#endif
public:
#if defined(FABLE_RETAIL_SUBTITLE_LIFETIME) || defined(FABLE_RETAIL_MINIMAP_LIFETIME)
    ~CBasicString(){DeallocateData();}
#endif
#ifdef FABLE_RETAIL_BODYGUARD_IDLE
    bool operator==(const T* pdata) const { return Compare(PData,pdata) ? false : true; }
#endif
    const T* PeekData() const { return PData; }
#ifdef FABLE_RETAIL_FACTIONS
    bool operator<(const CBasicString&)const;
#endif
#ifdef FABLE_RETAIL_SUBTITLE
    bool operator<(const CBasicString& other)const{return Compare(PData,other.PData)<0;}
#endif
#if defined(FABLE_RETAIL_NAVIGATION_UPDATE) || defined(FABLE_RETAIL_RELATIVE_PATH) || defined(FABLE_RETAIL_SHAPE_PERSIST)
    bool operator==(const CBasicString& other) const { if(StringLength!=other.StringLength)return false; return !Compare(PData,other.PData); }
#endif
#if defined(FABLE_RETAIL_BODYGUARD) || defined(FABLE_RETAIL_NAVIGATION_UPDATE) || defined(FABLE_RETAIL_SUBTITLE) || defined(FABLE_RETAIL_FACTIONS) || defined(FABLE_RETAIL_ANIMATION_SEQUENCE)
    unsigned long GetLength() const { return StringLength; }
#endif
#if defined(FABLE_RETAIL_NAVIGATION_UPDATE) || defined(FABLE_RETAIL_PERSIST_OPINION) || defined(FABLE_RETAIL_CTHINGMANAGER_SAVE) || defined(FABLE_RETAIL_MINIMAP_TRAITS) || defined(FABLE_RETAIL_REGION_GRAPH)
    bool operator==(const T* text) const;
#endif
};
#ifdef FABLE_RETAIL_FACTIONS
template<> bool CBasicString<char>::operator<(const CBasicString<char>&) const;
#endif
class CCharStringData {
public:
#if defined(FABLE_RETAIL_SUBTITLE_LIFETIME) || defined(FABLE_RETAIL_MINIMAP_LIFETIME)
    void DecRef(){--NoRefs;}
    long GetRefs()const{return NoRefs;}
    ~CCharStringData(){}
#endif
    CBasicString<char> Data;
protected:
    long NoRefs;
};
#else
class CCharStringData
{ public:
    char* text;
    fable_u32 unknown04;
    fable_u32 unknown08;
    fable_u8 flags0C;
    fable_i32 owners;
};
#endif
#pragma pack(pop)

#ifndef FABLE_RETAIL_CCHARSTRING_DATA

class CCharString;

class CWideString
{
public:
    CWideString();
    CWideString(const CWideString& other);
    explicit CWideString(const wchar_t* text);
    ~CWideString();
    operator const wchar_t*() const;
    const wchar_t* GetAsCharArray() const;                     // 0x0099AF10 ?GetAsCharArray@CWideString@@QBEPBGXZ
    long GetLength() const;
    long FindCharPos(wchar_t c) const;                         // 0x0099B070 ?FindCharPos@CWideString@@QBEJG@Z
    void __cdecl Sprintf(const wchar_t* format, ...);          // 0x0099BA70 ?Sprintf@CWideString@@QAAXPBGZZ
    void SetLength(long length);                               // 0x0099BC50 ?SetLength@CWideString@@QAEXJ@Z
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

protected:
    CWideStringData* AllocStringData(                          // 0x0099B3C0 protected (Ego_r / FableWin: IAE)
        const wchar_t* text,
        long length);

private:

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
    CCharString(const CCharString& other);                     // 0x0099EC30 ??0CCharString@@QAE@ABV0@@Z (Ego_r / FableWin)
    ~CCharString();
    operator const char*() const;
    const CCharString& operator=(const CCharString& other);
    const CCharString& operator=(const char* text);
    CWideString ToWideString() const;

protected:
    void UnassignString();                                     // 0x0099E9B0 protected (Ego_r / FableWin: IAE)
    CCharStringData* AllocStringData(                          // 0x0099EA60 protected (Ego_r / FableWin: IAE)
        const char* text,
        long length);

private:
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

#endif
