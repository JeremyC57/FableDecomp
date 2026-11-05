#pragma once

#include <stddef.h>

#include "rebuild_abi.h"
#ifdef FABLE_RETAIL_CCHARSTRING
#include <string.h>
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
#ifdef FABLE_RETAIL_CBASICSTRING_EQ_OUTOFLINE
    // Retail opt-in (2026-11-03, chunk 720 row 0x0065B854): the CTCVillage TU calls ??8?$CBasicString@D@@QBE_NABV0@@Z out of line
    // (0x004A93C0: the other PStringData pushed, ecx = this PStringData) from an inlined CCharString::operator== (0x0065B956..71);
    // with the in-class body VC7.1 folds it into CCharString::operator== and calls that instead. Declaration only.
    bool operator==(const CBasicString& other) const;                 // 0x004A93C0 (CBasicString<char>)
#else
    bool operator==(const CBasicString& other) const { if(StringLength!=other.StringLength)return false; return !Compare(PData,other.PData); }
#endif
    // Ego_r 0x0040C910 / FableWin 0x01899D90 ??8?$CBasicString@D@@QBE_NPBD@Z (retail out-of-line copy 0x004115A0; a TU that holds
    // the template body expands it, 0x009BC120).
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
    // Codex pass 13: ?SaveToFile@CCharString@@QBEXAAVCAFile@@@Z (Ego_r + FableWin), ?EqualsCaseInsensitive@CCharString@@QBE_NABV1@@Z and
    // ??MCCharString@@QBE_NABV0@@Z (FableWin; ??M bound by a landed row); all out of line in retail.
    void SaveToFile(class CAFile&) const;
    // Chunk 734 (row 0x004A0D90 CWorld::LoadQuestsFromFile): the body is inline -- retail expands it there (`_stricmp` on the two
    // GetAsCharArray results, 0x004A1028..0x004A105B; no out-of-line copy in Fable.exe or the manifest; FableWin's is the debug COMDAT).
#ifdef FABLE_RETAIL_CCHARSTRING_FORCEINLINE_EQUALSCI
    // Per-TU inline opt-in (chunk 779, row 0x00837E40 CThingTrackNode::ConstructFromLevelScript; like FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ):
    // that TU expands all four EqualsCaseInsensitive uses in place (0x00838195..0x008381C9: both GetAsCharArray ternaries, `call _stricmp`,
    // `neg ; sbb ; inc`) where the plain inline body is called out of line once the eight inlined key compares have used the budget.
    __forceinline
#endif
    bool EqualsCaseInsensitive(const CCharString& other) const { return _stricmp(GetAsCharArray(), other.GetAsCharArray()) == 0; }
    bool operator<(const CCharString&) const;
#ifdef FABLE_RETAIL_CBASICSTRING_EQ_OUTOFLINE
    // (same opt-in) retail expands CCharString::operator== in place around the CBasicString call even in the size-optimised
    // CTCVillage TU (0x0065B956..0x0065B97A), where plain inline is called out of line.
    __forceinline
#endif
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
    // GetLength: the null test as `PStringData==0 ? 0 : ...` lets VC7.1 reuse the zero register (0x00E5EBD0 `test; je; mov eax,[eax+4];
    // dec eax`; the `PStringData ? ... : 0` form adds `jmp; xor eax,eax`, 1068v1064); 0x00521620 / 0x00522720 are byte-identical either way.
    long GetLength() const { return PStringData==0 ? 0 : (long)PStringData->Data.GetLength(); }
    bool IsEmpty() const { return GetLength() == 0; }
    // Ego_r 0x004037B0 / FableWin 0x0186C770 ?GetAsCharArray@CCharString@@QBEPBDXZ and FableWin 0x01899D30
    // ??8CCharString@@QBE_NPBD@Z, both inline in retail (0x00AE8D50 expands GetAsCharArray: `test; jne; mov eax,offset NullChar; jmp; mov eax,[eax]`;
    // the landed local models spell the same ternary). NullChar is 0x0129AAF4 (static data member per the Ego_r TPI as Codex pass 12 read it).
    const char* GetAsCharArray() const { return PStringData==0 ? &NullChar : PStringData->Data.PeekData(); }
#ifdef FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ
    // Per-TU inline opt-in (Codex pass 13, row 0x00E36D50; like FABLE_RETAIL_CBASICSTRING_EQ_OUTOFLINE): that TU expands both
    // null / non-null arms in place where the plain inline operator is called out of line.
    __forceinline
#endif
    bool operator==(const char* text) const { if(!PStringData) { if(strcmp(text,"")==0) return true; return false; } return PStringData->Data==text; }
#ifdef FABLE_RETAIL_CCHARSTRING_CHAR_EQ
    // Nested opt-in (chunk 778, row 0x00615E70 CPersistTraits<CTCInventoryMap::CUIRegionInfo>::TransferIn): FableWin.pdb
    // ??8CCharString@@QBE_ND@Z (0x020C8690; Ego_r keeps only ??9CCharString@@QBE_ND@Z, retail's out-of-line operator!= 0x0099E520).
    // Inline in retail: a null string is never equal; else {character, 0} is built on the stack and compared with the strcmp
    // intrinsic (0x006162B4: `mov byte ptr [esp+4Ch],2Ch ; mov byte ptr [esp+4Dh],0`, bytewise loop, `sbb ; sbb ; sete`).
    // __forceinline: the landed 0x0099E520 / 0x00997DA0 / 0x00997E90 local models found plain inline called out of line.
    __forceinline bool operator==(char character) const { if(PStringData!=0) { char text[2]; text[0]=character; text[1]=0; return strcmp(PStringData->Data.PeekData(),text)==0 ? true : false; } return false; }
#endif
#ifdef FABLE_RETAIL_CCHARSTRING_WHITESPACE
    // Nested opt-in (chunk 775, row 0x009A8170): Ego_r.pdb ?RemoveWhiteSpaceBeginEnd@CCharString@@QAEXXZ (0x0099F560), out of line.
    void RemoveWhiteSpaceBeginEnd();
#endif
    void Truncate(unsigned long);                              // 0x0099F2A0 ?Truncate@CCharString@@QAEXK@Z (Ego_r 0x00AFAB00 / FableWin 0x02F61D70)
    void LoadFromFile(class CAFile&,class CASuspendableProcess*,long);   // ?LoadFromFile@CCharString@@QAEXAAVCAFile@@PAVCASuspendableProcess@@J@Z (Ego_r)
#ifdef FABLE_RETAIL_CCHARSTRING_SUBSTRING
    // Chunk 759 (rows 0x00870E00 / 0x00871290 NCreatureAction::CActionEvent*::SetDataFromString): Ego_r.pdb + FableWin.pdb
    // ?FindCharPos@CCharString@@QBEJD@Z and ?GetSubString@CCharString@@QBE?AV1@KK@Z, both out of line in retail.
    long FindCharPos(char c) const;                            // 0x0099E5A0
    CCharString GetSubString(unsigned long start, unsigned long length) const;   // 0x0099EC70
#endif
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
    static const char NullChar;                                // Ego_r TPI CCharString (protected static); GetAsCharArray's empty result
    CCharStringData* PStringData;                              // +0x00 (Ego_r TPI 0x27dc: protected PStringData)
#else
private:
    CCharStringData* storage_;
#endif
};

CCharString FABLE_FASTCALL operator+(
    const CCharString& left,
    const char* right);
#ifdef FABLE_RETAIL_CCHARSTRING
// Codex pass 13: the free ??H@YI?AVCCharString@@PBDABV0@@Z (Ego_r + FableWin; `const char*` + CCharString, __fastcall).
CCharString FABLE_FASTCALL operator+(const char*,const CCharString&);
#endif
#ifdef FABLE_RETAIL_CCHARSTRING_CONCAT_STRING
// Chunk 772: the free ??H@YI?AVCCharString@@ABV0@0@Z (Ego_r + FableWin; CCharString + CCharString, __fastcall), retail 0x0099F570:
// NScript spawn names `CCharString("ParentClass.") + CCharString("<Func>")` (e.g. 0x00E1F86F, 0x00E0DAA0's row).
CCharString FABLE_FASTCALL operator+(const CCharString&,const CCharString&);
#endif

// Retail folds equivalent narrow/wide string lifetime helpers. These counters
// remain separate because their addresses and object families are distinct.
extern fable_i32 g_CWideStringInstanceCount_013BCA20;
extern fable_i32 g_CCharStringInstanceCount_013BD800;
extern wchar_t g_FableEmptyWideString_0129A8E0[1];

FABLE_STATIC_ASSERT(sizeof(CWideStringData) == 0x10);
FABLE_STATIC_ASSERT(sizeof(CCharStringData) == 0x11);
FABLE_STATIC_ASSERT(sizeof(CWideString) == 0x04);
FABLE_STATIC_ASSERT(sizeof(CCharString) == 0x04);
