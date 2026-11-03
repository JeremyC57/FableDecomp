#pragma once

#include <stddef.h>

#include "rebuild_abi.h"
#ifdef FABLE_RETAIL_CCHARSTRING
#include <string.h>
#pragma intrinsic(strcmp)
#endif

#ifdef FABLE_RETAIL_CWIDESTRING
// Retail CWideStringData (Ego_r TPI: protected Data, public NoRefs at +0x0C, const operator==); the STLport
// basic_string<unsigned short> is the three-pointer string of stlport/fable_string.h (0x10 bytes in all).
#include "stlport/fable_string.h"
class CWideStringData
{
protected:
    std::basic_string<unsigned short> Data;
public:
    long NoRefs;
    bool operator==(const CWideStringData& other) const { const std::basic_string<unsigned short>& other_data=other.Data; return Data==other_data; }
};
#else
class CWideStringData
{ public:
    wchar_t* text;
    fable_u32 unknown04;
    fable_u32 unknown08;
    fable_i32 owners;
};
#endif

#pragma pack(push, 1)
#ifdef FABLE_RETAIL_CCHARSTRING
// Retail opt-in (Codex pass 11 consolidation): Ego_r TPI CCharStringData = { CBasicString<char> Data; long NoRefs; } and
// CBasicString<T> = { T* PData; unsigned long StringLength; unsigned long DataLength : 31; bool UseFastExtend : 1; },
// 13 + 4 = 0x11 bytes in the existing packed scope (the same sizeof as the default stand-in below). PeekData and the
// length-then-Compare equality are inline in both PDBs' callers (0x00998570 GetRelativeFilePath compares two paths inline).
template<class T> class CBasicString {
protected:
    T* PData;
    unsigned long StringLength;
    unsigned long DataLength : 31;
    bool UseFastExtend : 1;
    static long __fastcall Compare(const T*,const T*);
public:
    const T* PeekData() const { return PData; }
    unsigned long GetLength() const { return StringLength; }
    bool operator==(const CBasicString& other) const { if(StringLength!=other.StringLength)return false; return !Compare(PData,other.PData); }
    // Retail opt-in addition (2026-11-03, chunk 719 row 0x00509A20, agent719c): the out-of-line ??8?$CBasicString@D@@QBE_NPBD@Z 0x004115A0
    // (Ego_r.pdb / FableWin.pdb; its body inlines Compare), called by the inline CCharString::operator==(const char*) below.
    bool operator==(const T* text) const;
};
class CCharStringData
{ public:
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

class CCharString;

class CWideString
{
public:
    CWideString();
    CWideString(const CWideString& other);
#ifdef FABLE_RETAIL_CWIDESTRING
    // Retail converts bare wide literals implicitly (0x006286F0 Replace(L".xmv", L".wmv"): each temporary's address is
    // re-taken with `lea` after the 0x0099B6B0 call, the implicit-conversion shape; an explicit CWideString(L"...")
    // argument pushes the constructor's eax instead), so the retail declaration is not `explicit`.
    CWideString(const wchar_t* text);                          // 0x0099B6B0 ??0CWideString@@QAE@PBG@Z
#else
    explicit CWideString(const wchar_t* text);
#endif
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
#ifdef FABLE_RETAIL_CWIDESTRING
    CCharString ToCharString() const;                          // ?ToCharString@CWideString@@QBE?AVCCharString@@XZ
    bool operator==(const CWideString& other) const            // inline in retail (FableWin ??8CWideString@@QBE_NABV0@@Z)
    {
        if(PStringData==other.PStringData)return true;
        if(!PStringData || !other.PStringData)return false;
        return *PStringData==*other.PStringData;
    }
    bool Contains(const CWideString& other) const;             // ?Contains@CWideString@@QBE_NABV1@@Z
    void Replace(const CWideString& search, const CWideString& replacement);   // 0x0099C1E0 ?Replace@CWideString@@QAEXABV1@0@Z (Ego_r + FableWin)
#endif

private:
    explicit CWideString(const CCharString& text);
#ifndef FABLE_RETAIL_CWIDESTRING
    void UnassignString();
#endif

protected:
    CWideStringData* AllocStringData(                          // 0x0099B3C0 protected (Ego_r / FableWin: IAE)
        const wchar_t* text,
        long length);

#ifdef FABLE_RETAIL_CWIDESTRING
    static long NoStrings;
    // FableWin ?UnassignString@CWideString@@IAEXXZ (protected; inline in retail)
    void UnassignString() { if(PStringData) { if(--PStringData->NoRefs<=0)delete PStringData;PStringData=0; } }
    CWideStringData* PStringData;                              // +0x00 (Ego_r TPI: protected PStringData)
#else
private:

    CWideStringData* storage_;
#endif
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
#ifdef FABLE_RETAIL_CCHARSTRING
    // Retail opt-in (chunk 717): retail's only const char* constructor is ??0CCharString@@QAE@PBDJ@Z 0x0099EBF0 (Ego_r.pdb +
    // FableWin.pdb; no one-argument overload exists in either PDB or in Fable.exe), yet retail converts bare literals
    // implicitly through it (`push -1 ; push <lit>` at every in-place construction: braced CCharString arrays 0x0121E260 /
    // 0x0121E3A0, vector<CCharString>(18, "") 0x0121A460 which matches only through the implicit conversion, a const
    // CCharString& bound to a literal 0x0121EF70): the declaration carries the default length -1.
    CCharString(const char* text, long length = -1);           // 0x0099EBF0
    // Retail opt-in (Codex pass 11 consolidation): further Ego_r CCharString members used inline by retail callers.
    unsigned long GetCRC() const;                              // 0x00404310 (Ego_r 0x00403C00)
    bool operator==(const CCharString& other) const {
        if(PStringData==other.PStringData)return true;
        if(!PStringData||!other.PStringData)return false;
        return PStringData->Data==other.PStringData->Data;
    }
    bool operator!=(const CCharString&) const;
    bool operator!=(const char*) const;
    const CCharString& operator+=(const char*);
    const CCharString& operator+=(const CCharString&);
    // Inline in retail (no out-of-line copy is bound): ?GetLength@CCharString@@QBEJXZ (Ego_r + FableWin), ?IsEmpty@CCharString@@QBE_NXZ
    // (FableWin 0x01D34B90); 0x00522720 tests '!PStringData || !StringLength' in place.
    // The null test is negated in retail (2026-11-03, chunk 719 row 0x00509A20, agent719c): `jne` over the zero arm when the length is
    // stored (0x00509FE4) and the zero reused from the null pointer in a compare (0x0050A00D); the `p ? len : 0` form gives neither.
    long GetLength() const { return !PStringData ? 0 : (long)PStringData->Data.GetLength(); }
    bool IsEmpty() const { return GetLength() == 0; }
    // Retail opt-in additions (2026-11-03, chunk 719 row 0x00509A20, agent719c): ?GetSubString@CCharString@@QBE?AV1@KK@Z 0x0099EC70
    // (Ego_r.pdb / FableWin.pdb), and ??8CCharString@@QBE_NPBD@Z inline (no out-of-line copy bound in Fable.exe): the null-data arm is
    // the intrinsic `strcmp(text, "")` (`repe cmpsb` against "" with the literal's length + 1, 0x00509E13; landed 0x0083BF90 has the
    // same arm), the other calls CBasicString<char>::operator==(const char*) 0x004115A0.
    CCharString GetSubString(unsigned long start, unsigned long length) const;   // 0x0099EC70
    bool operator==(const char* text) const {
        if(PStringData==0) {
            if(strcmp(text,"")==0)return true;
            return false;
        }
        return PStringData->Data==text;
    }
#else
    CCharString(const char* text, long length);
#endif
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

#ifdef FABLE_RETAIL_CCHARSTRING
    static long NoStrings;
    CCharStringData* PStringData;                              // +0x00 (Ego_r TPI 0x27dc: protected PStringData)
#else
private:
    CCharStringData* storage_;
#endif
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
