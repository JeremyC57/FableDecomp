#pragma once
#include "fable_compiler.h"
#ifdef FABLE_RETAIL_CCHARSTRING_STARTSWITH_ASSERT
#include "fable_assert.h"
#endif

#include <stddef.h>

#include "rebuild_abi.h"
#ifdef FABLE_RETAIL_SHOVE_STRING_CLEANUP_PASS23
// VC7.1 RTM SDK <new>: exact array deallocator declaration. Legacy fallback
// CRT headers omit it and lower delete[] to scalar delete; nativeBFEB1C is??_V.
void __cdecl operator delete[](void*) throw();
#endif
#ifdef FABLE_RETAIL_CCHARSTRING
#include <string.h>
#endif
#ifdef FABLE_RETAIL_CCHARSTRING_GETCRC_INLINE
#include "engine/CCRC.h"
#endif

#ifdef FABLE_RETAIL_CWIDESTRING
// Retail CWideStringData (Ego_r TPI: protected Data, public NoRefs at +0x0C, const operator==); the STLport
// basic_string<unsigned short> is the three-pointer string of stlport/fable_string.h (0x10 bytes in all).
#include "stlport/fable_string.h"
#if defined(FABLE_RETAIL_CWIDESTRING_TEXT_EQ) && !defined(FABLE_RETAIL_CWIDESTRING_TEXT_EQ_STLP_INLINE)
// (chunk 784) the wide text comparison is out of line in retail (0x00404280; char_traits<unsigned short>::length has no body here).
namespace std { template<> bool operator==(const basic_string<unsigned short>&,const unsigned short*); }
#endif
// FABLE_RETAIL_CWIDESTRING_TEXT_EQ_STLP_INLINE (nested in FABLE_RETAIL_CWIDESTRING_TEXT_EQ; chunk 2032, row 0x009C47A0
// CFileInstaller::AddFile): that TU expands the STLport text operator== under CWideString::operator==(const wchar_t*) instead of
// calling 0x00404280. The literal's length folds to 0 (define FABLE_RETAIL_STLP_CHAR_TRAITS_LENGTH for STLport's
// __char_traits_base::length), so retail tests `(finish - start) & ~1` and calls compare 0x00403C60 with n = 0 (0x009C4C9A..B5).
class CWideStringData
{
protected:
    std::basic_string<unsigned short> Data;
public:
    long NoRefs;
#ifdef FABLE_RETAIL_CWIDESTRINGDATA_CTOR_TEXT
    // Opt-in (NF220, q12401: row 0x0099C120): Ego_r.pdb / FableWin.pdb ??0CWideStringData@@QAE@PB_WJ@Z (public). Declaration only.
    CWideStringData(const wchar_t* text, long length);
#endif
#ifdef FABLE_RETAIL_CWIDESTRINGDATA_APPEND_CHAR
    // Opt-in (NF220, q12401: row 0x0099C120): Ego_r.pdb / FableWin.pdb ?Append@CWideStringData@@QAEX_W@Z (public). Declaration only.
    void Append(wchar_t character);
#endif
#ifdef FABLE_RETAIL_CWIDESTRING_REPLACE_PASS24
    void Append(const wchar_t* text,long length) {
        if (length < 0) Data.append(text);
        else Data.append(text, text + length);
    }
#endif
    bool operator==(const CWideStringData& other) const { const std::basic_string<unsigned short>& other_data=other.Data; return Data==other_data; }
#ifdef FABLE_RETAIL_CWIDESTRING_TOCHARSTRING_BODY
    // Retail opt-in (NF80, q10939: row 0x0099B2C0 CWideString::ToCharString): Ego_r.pdb ?GetLength@CWideStringData@@QBEJXZ and
    // ?PeekStringData@CWideStringData@@QAEPB_WXZ (public; the STLport string's finish - start and start), expanded in retail under
    // the inline CWideString accessors below (0x0099B2D0..0x0099B2D5 `mov eax,[ecx+4] ; sub eax,[ecx] ; sar eax,1`, 0x0099B32C).
    long GetLength() const { return Data.size(); }
    const wchar_t* PeekStringData() { return Data.c_str(); }
#endif
#ifdef FABLE_RETAIL_CWIDESTRING_TEXT_EQ
    // Retail opt-in (chunk 784, row 0x00491AD2 CSubtitleRenderer::ProcessEmbeddedHighlightTags): the text comparison under the
    // inline CWideString::operator==(const wchar_t*) below; retail calls the STLport operator== 0x00404280 out of line
    // (??$?8GV?$char_traits@G@std@@V?$allocator@G@1@@std@@YA_NABV?$basic_string@GV?$char_traits@G@std@@V?$allocator@G@2@@0@PBG@Z).
#ifdef FABLE_RETAIL_CWIDESTRING_FORCEINLINE_DATA_TEXT_EQ
    // Nested opt-in (chunk 886, row 0x0041E6D3 NUISystem::CManager::Action): with FABLE_RETAIL_CWIDESTRING_FORCEINLINE_EQ that
    // TU also expands this body inside the expanded CWideString::operator==(const wchar_t*) (0x00424F4A / 0x004262A0:
    // `push literal ; push &Data ; call 0x00404280 ; pop ecx ; pop ecx`) where the inline budget would call it.
    FABLE_FORCEINLINE
#endif
    bool operator==(const unsigned short* text) const { return Data==text; }
#endif
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
#if defined(FABLE_RETAIL_CBASICSTRING_COMPARE_BODY)
    // Opt-in (row 0x009C9280 NGameText::CDataBank::GetTextEntryFromGroup): the template body of
    // ?Compare@?$CBasicString@D@@KIJPBD0@Z (out of line at 0x00411570, landed CBasicStringchar_Compare_00411570.cpp) is visible to the
    // caller's TU; VC7.1 keeps calling it (the loop is not inlined) but then knows the call writes no memory, so the caller keeps a
    // loaded array base across it (0x009C937F `mov edi,[esi+204h]` reused at 0x009C93D7 after both Compare calls).
#ifdef FABLE_RETAIL_BEARDY_ORIGINAL_TEXT_COMPARISON_PASS27
    static long __fastcall Compare(const T* str1,const T* str2)
    {
        while (*str1 || *str2) {
            if (*str1 < *str2) return -1;
            else if (*str1 > *str2) return 1;
            ++str1;
            ++str2;
        }
        return 0;
    }
#else
    static long __fastcall Compare(const T* a,const T* b)
    {
        for (;;) {
            T ca = *a;
            if (ca == 0 && *b == 0)
                return 0;
            T cb = *b;
            if (ca < cb)
                return -1;
            if (ca > cb)
                return 1;
            ++a;
            ++b;
        }
    }
#endif
#else
    static long __fastcall Compare(const T*,const T*);
#endif
#ifdef FABLE_RETAIL_SHOVE_STRING_CLEANUP_PASS23
    // Both original PDBs: protected DeallocateData, native expanded in0099E9B0.
    // FableWin02F650E0 array-deletes only nonnull data and clears that pointer.
    void DeallocateData() { if(PData) { delete[] PData; PData=0; } }
#endif
public:
#ifdef FABLE_RETAIL_SHOVE_STRING_CLEANUP_PASS23
    // Original02F65050 calls DeallocateData then clears lengths, preserving both flag bits.
    ~CBasicString() { DeallocateData(); StringLength=0; DataLength=0; }
#endif
#ifdef FABLE_RETAIL_CCHARSTRING_FORMAT_PASS29
protected:
    void SetString(const T*, unsigned long);
public:
    CBasicString(const T*);
    // AllocStringData expands both block constructors at 0099eaa3.
    FABLE_FORCEINLINE CBasicString(const T* string, unsigned long length)
        : PData(0), StringLength(0), DataLength(0), UseFastExtend(1) { SetString(string, length); }
    void Append(T, unsigned long);
    void Truncate(unsigned long);
#endif
#if defined(FABLE_RETAIL_SHOP_STRING_DATA_CTORS26) && !defined(FABLE_RETAIL_CCHARSTRING_FORMAT_PASS29)
protected:
    void SetString(const T*, unsigned long);
public:
    CBasicString(const T*);
    // Retail 0099EAA3 expands the explicit-length data constructor and its member.
    FABLE_FORCEINLINE CBasicString(const T* text, unsigned long length)
        : PData(0), StringLength(0), DataLength(0), UseFastExtend(true)
    {
        SetString(text, length);
    }
#endif
    const T* PeekData() const { return PData; }
    unsigned long GetLength() const { return StringLength; }
#ifdef FABLE_RETAIL_CBASICSTRING_EQ_OUTOFLINE
    // Retail opt-in (2026-11-03, chunk 720 row 0x0065B854): the CTCVillage TU calls ??8?$CBasicString@D@@QBE_NABV0@@Z out of line
    // (0x004A93C0: the other PStringData pushed, ecx = this PStringData) from an inlined CCharString::operator== (0x0065B956..71);
    // with the in-class body VC7.1 folds it into CCharString::operator== and calls that instead. Declaration only.
    bool operator==(const CBasicString& other) const;                 // 0x004A93C0 (CBasicString<char>)
#else
#ifdef FABLE_RETAIL_CCHARSTRING_FORCEINLINE_EQ
    // (chunk 795 opt-in, see CCharString::operator== below)
    FABLE_FORCEINLINE
#endif
    bool operator==(const CBasicString& other) const { if(StringLength!=other.StringLength)return false; return !Compare(PData,other.PData); }
#endif
    // Ego_r 0x0040C910 / FableWin 0x01899D90 ??8?$CBasicString@D@@QBE_NPBD@Z (retail out-of-line copy 0x004115A0; a TU that holds the template body expands it, 0x009BC120).
#if defined(FABLE_RETAIL_CBASICSTRING_TEXT_EQ_COMPARE_ZERO)
#ifdef FABLE_RETAIL_BEARDY_ORIGINAL_TEXT_COMPARISON_PASS27
    // Inline-budget: 00E4FB8F calls the 40-byte Compare inside two expanded equalities.
    // The 88-byte standalone equality at 004115A0 expands Compare one level below its own entry.
    FABLE_FORCEINLINE bool operator==(const T* text) const {
#ifdef _MSC_VER
#pragma inline_depth(1)
#endif
        return Compare(PData,text)==0;
#ifdef _MSC_VER
#pragma inline_depth()
#endif
    }
#else
    bool operator==(const T* text) const { return Compare(PData,text)==0; }   // Codex pass 22: native 0x004115A0 keeps the original compare-result == 0.
#endif
#elif defined(FABLE_RETAIL_CBASICSTRING_TEXT_EQ_INLINE)
    // Retail opt-in (chunk 780): the quest-script TUs expand CBasicString<char>::operator==(const char*) in place as
    // `call Compare 0x00411570 ; neg eax ; sbb al,al ; inc al` (CV_BeardyBaldyScript::Main 0x00E4FB88..0x00E4FB98, the landed
    // chunk 778 WatchForQuestCardConditions model), i.e. Compare(PData, text) == 0 (FableWin ??8?$CBasicString@D@@QBE_NPBD@Z).
    bool operator==(const T* text) const { return Compare(PData,text) ? false : true; }
#elif defined(FABLE_RETAIL_CBASICSTRING_TEXT_EQ_NOINLINE_COMPARE_ZERO)
    // Retail opt-in (NF215, 2026-11-21, q12262): the out-of-line body of 0x004115A0 visible to a caller that calls it (like
    // FABLE_RETAIL_CBASICSTRING_TEXT_EQ_NOINLINE), spelled as the owner row is (Compare-result == 0). With FABLE_RETAIL_CBASICSTRING_COMPARE_BODY
    // the pick-any COMDAT the caller's object carries is the owner's 88-byte body (Compare expanded, `xor ecx,ecx ; test eax,eax ; sete cl`
    // on each return path) instead of the 76-byte `? false : true` copy.
    __declspec(noinline) bool operator==(const T* text) const { return Compare(PData,text)==0; }
#elif defined(FABLE_RETAIL_CBASICSTRING_TEXT_EQ_NOINLINE)
    // Retail opt-in (row 0x007D2CC0 CTCTavernGameCoinGolf::EnableCourse): the template body visible but called out of line
    // (0x004115A0 COMDAT): VC7.1 then knows it writes no memory, so the caller's string pointer survives the call (no second null test).
    __declspec(noinline) bool operator==(const T* text) const { return Compare(PData,text) ? false : true; }
#else
    bool operator==(const T* text) const;
#endif
#ifdef FABLE_RETAIL_CCHARSTRING_LESS_INLINE
    // Retail opt-in (chunk 846, row 0x00520D00 via the set<CCharString> _M_find 0x004B9370): Ego_r.pdb ??M?$CBasicString@D@@QBE_NABV0@@Z,
    // out of line at 0x00429950 (landed; _M_find calls it at 0x004B9395). Declaration only.
    bool operator<(const CBasicString& other) const;                 // 0x00429950 (CBasicString<char>)
#endif
};
#ifdef FABLE_RETAIL_CBASICSTRING_CHAR_COMPARE_MINIMAP_PASS26
// FableWin 01899DC0 and 01899D90, retail 00411570 and 004115A0.
// The two signed character operands are read again for the ordering tests.
#ifdef _MSC_VER
#pragma optimize("",on)
#endif
template<> inline long FABLE_FASTCALL CBasicString<char>::Compare(const char* a,const char* b)
{
    while (*a || *b) {
        if (*a < *b) return -1;
        else if (*a > *b) return 1;
        ++a;
        ++b;
    }
    return 0;
}
template<> inline bool CBasicString<char>::operator==(const char* text) const
{
    return Compare(PData,text)==0;
}
#ifdef _MSC_VER
#pragma optimize("s",on)
#endif
#endif // FABLE_RETAIL_CBASICSTRING_CHAR_COMPARE_MINIMAP_PASS26
#if defined(FABLE_RETAIL_CCHARSTRING_LESS_INLINE) && defined(FABLE_RETAIL_CBASICSTRING_LESS_BODY)
// Retail opt-in (NF80, q10763 / q10923): the one definition of Ego_r.pdb ??M?$CBasicString@D@@QBE_NABV0@@Z (0x00429950), as a
// template member so every object that sees it emits a pick-any COMDAT (selection 2) and the copies link together. Never inlined:
// the includer still calls 0x00429950 but sees that it writes no memory (0x009EFB80 insert_unique and the _M_find rows keep the key
// in a register across the call). The self row 0x00429950 explicitly instantiates it. Retail inlines the three-way compare and
// tests its result `< 0` on each of the three return paths (`xor ecx,ecx ; test eax,eax ; setl cl`); the other string's pointer is
// read first and is the loop's induction pointer.
template<class T>
#ifdef _MSC_VER
__declspec(noinline)
#endif
bool CBasicString<T>::operator<(const CBasicString<T>& other) const
{
    const T* b = other.PData;
    const T* a = PData;
    long result;
    for (;;) {
        if (*a == 0 && *b == 0) { result = 0; break; }
        if (*a < *b) { result = -1; break; }
        if (*a > *b) { result = 1; break; }
        ++a;
        ++b;
    }
    return result < 0;
}
#ifdef FABLE_RETAIL_MENU_BASICSTRING_SPEED_OWNER_PASS29
// The native 00429950 lexicographic comparison keeps its speed setting in size-built menu code.
template<> inline bool CBasicString<char>::operator<(const CBasicString<char>& other) const
{
    const char* b = other.PData;
    const char* a = PData;
    long result;
    for (;;) {
        if (*a == 0 && *b == 0) { result = 0; break; }
        if (*a < *b) { result = -1; break; }
        if (*a > *b) { result = 1; break; }
        ++a;
        ++b;
    }
    return result < 0;
}
#endif
#endif
#if defined(FABLE_RETAIL_CBASICSTRING_TEXT_EQ_OWNER_BODY) && defined(FABLE_RETAIL_CBASICSTRING_COMPARE_BODY)
// NF220 opt-in (2026-11-21, q12383; rows 0x005A4CDD, ...). Fidelity: scaffolding, not retail. A size-compiled TU that calls
// CBasicString<char>::operator==(const char*) out of line carries a pick-any copy of ??8?$CBasicString@D@@QBE_NPBD@Z; as an implicit
// template instantiation VC7.1 compiles it in the optimize mode in force at the END of the TU (NF216 FINDINGS 2026-11-21), so a size TU
// emits a 19 / 20-byte copy. The advisor pick-any rule (EXTRA_RULES 2026-11-20) needs the owner's body: the 88-byte copy of row
// 0x004115A0 (catalog flags: the default /O2 /Oy /W3; Compare expanded, `xor ecx,ecx ; test eax,eax ; sete cl` on each return). An
// explicit specialization takes the mode at its definition, so this one is compiled under optimize("ty", on), the owner's
// command-line mode (EXTRA_RULES 2026-11-21, size-pragma condition 2), noinline as retail calls it. `#pragma optimize("", on)` below
// returns to the command-line settings (VC7.1 has no push / pop for optimize), so a row that uses this opt-in must not carry a
// row-head optimize pragma before its include of this header (nf189). Needs FABLE_RETAIL_CBASICSTRING_COMPARE_BODY and none of the
// FABLE_RETAIL_CBASICSTRING_TEXT_EQ_* bodies (the declaration-only operator== above); place before any use of this operator.
#pragma optimize("ty", on)
template<> inline
#ifdef _MSC_VER
__declspec(noinline)
#endif
bool CBasicString<char>::operator==(const char* text) const { return Compare(PData,text)==0; }   // 0x004115A0
#pragma optimize("", on)
#endif
class CCharStringData
{ public:
    CBasicString<char> Data;
#if defined(FABLE_RETAIL_SHOP_STRING_DATA_CTORS26) && !defined(FABLE_RETAIL_CCHARSTRING_FORMAT_PASS29)
    CCharStringData(const char* text) : Data(text), NoRefs(1) {}
    FABLE_FORCEINLINE CCharStringData(const char* text, long length) : Data(text, length), NoRefs(1) {}
#endif
#ifdef FABLE_RETAIL_CCHARSTRING_FORMAT_PASS29
    CCharStringData(const char* string) : Data(string), NoRefs(1) { }
    FABLE_FORCEINLINE CCharStringData(const char* string, long length) : Data(string, length), NoRefs(1) { }
    void IncRef() { ++NoRefs; }
#endif

#ifdef FABLE_RETAIL_SHOVE_STRING_CLEANUP_PASS23
    // Both PDBs CCharStringData16members; native0099E9BA..99E9C8 count protocol.
    void DecRef() { --NoRefs; }
    long GetRefs() const { return NoRefs; }
    // Original02F62760 has an empty body; the Data member destructor follows.
    ~CCharStringData() { }
#endif
protected:
    long NoRefs;
#ifdef FABLE_RETAIL_CCHARSTRINGDATA_FRIEND
    // agentNF22 (row 0x0099EFB0 CCharString::operator=(const CCharString&)): NoRefs is protected in the Ego_r TPI, yet the
    // CCharString members share the data block by bumping it in place (operator= 0x0099EFB0 `inc dword ptr [eax+0Dh]`, the copy
    // constructor 0x0099EC30 the same) -- CCharString is a friend.
    friend class CCharString;
#endif
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

#ifdef FABLE_RETAIL_CWIDESTRING_TEXT_EQ_HELPER
// Nested opt-in (chunk 886, row 0x0041E6D3 NUISystem::CManager::Action): FableWin.pdb ?GFStringsAreEqual@@YI_NPB_W0@Z, the
// __fastcall wide-text equality the inline CWideString::operator==(const wchar_t*) uses for a null string. Retail calls its
// out-of-line COMDAT 0x00401490 from Action (0x00424EC2: ecx = the literal, edx = L"") where the 784 TU expands it in place
// (`push edx ; push ecx ; call wcscmp ; add esp,8 ; neg eax ; sbb eax,eax ; inc eax`).
inline bool FABLE_FASTCALL GFStringsAreEqual(const wchar_t* a, const wchar_t* b) { return wcscmp(a, b) == 0; }   // 0x00401490
#endif

class CWideString
{
public:
#if defined(FABLE_RETAIL_SUBTITLE_WRAP_BODY_PASS29) && !defined(FABLE_RETAIL_CWIDESTRING_REPLACE_PASS24)
    void SetAsSubString(const wchar_t* text, long length);
    // FableWin 01913970 calls GetAsCharArray before indexing the UTF-16 text.
    wchar_t GetChar(long index) const { return GetAsCharArray()[index]; }
#endif
#ifdef FABLE_RETAIL_STRING_MEMTRACKING_PASS26
    // Both original PDB PUBLIC/GDATA records; native string allocation category.
    static long MemTrackingID;
#endif
    CWideString();
#ifdef FABLE_RETAIL_CWIDESTRING_REPLACE_PASS24
    void SetAsSubString(const wchar_t* data, long length) { SetString(data,length); }
protected:
    void MakeOwnCopyOfData();
public:
#endif
#if defined(FABLE_RETAIL_CWIDESTRING) && defined(FABLE_RETAIL_CWIDESTRING_COPYCTOR_INLINE)
    // Opt-in (NF220, q12401: row 0x0099C120): the CWideString TU expands the copy constructor (owner 0x0099B720 ??0CWideString@@QAE@ABV0@@Z)
    // inside operator+ (0x0099C12A..: zero, NoStrings++, the self / same-block tests, UnassignString 0x0099B4D0 out of line, share + NoRefs++).
    CWideString(const CWideString& other) : PStringData(0) {
        ++NoStrings;
        if(&other!=this && other.PStringData!=PStringData){if(PStringData)UnassignString();CWideStringData* data=other.PStringData;if(data){PStringData=data;++data->NoRefs;}}
    }
#else
    CWideString(const CWideString& other);
#endif
#ifdef FABLE_RETAIL_CWIDESTRING
    // Retail converts bare wide literals implicitly (0x006286F0 Replace(L".xmv", L".wmv"): each temporary's address is
    // re-taken with `lea` after the 0x0099B6B0 call, the implicit-conversion shape; an explicit CWideString(L"...")
    // argument pushes the constructor's eax instead), so the retail declaration is not `explicit`.
    CWideString(const wchar_t* text);                          // 0x0099B6B0 ??0CWideString@@QAE@PBG@Z
#else
    explicit CWideString(const wchar_t* text);
#endif
#if defined(FABLE_RETAIL_CWIDESTRING) && defined(FABLE_RETAIL_CWIDESTRING_DTOR_INLINE)
    // Opt-in (NF220, q12401: row 0x0099C120): the CWideString TU expands the destructor (owner 0x0099B510 ??1CWideString@@QAE@XZ)
    // inside operator+ (0x0099C17B..: NoRefs--, on zero the STLport text freed and the block deleted, the pointer cleared, NoStrings--).
    // The owner row's own text (coordinator, landing nf220): its pick-any copy equals retail 0x0099B510 (68 = 68 B, relocations masked);
    // the shorter `if(--NoRefs<=0)` spelling gave a differing 65-B copy. 0x0099C120's bytes are the same with either.
    ~CWideString() { CWideStringData* storage = PStringData; if (storage != 0) { --storage->NoRefs; if (storage->NoRefs <= 0) { storage = PStringData; if (storage != 0) delete storage; } PStringData = 0; } --NoStrings; }
#else
    ~CWideString();
#endif
    operator const wchar_t*() const;
#if defined(FABLE_RETAIL_CWIDESTRING) && defined(FABLE_RETAIL_CWIDESTRING_TOCHARSTRING_BODY)
    // Retail opt-in (NF80, q10939: row 0x0099B2C0 CWideString::ToCharString): Ego_r.pdb / FableWin.pdb ?GetAsCharArray@CWideString@@QBEPB_WXZ
    // (out of line at 0x0099AF10) and ?GetLength@CWideString@@QBEJXZ, defined in the CWideString TU and expanded there: ToCharString
    // tests the data pointer for each length (0x0099B2C4..0x0099B2D5, `xor eax,eax` when null) and reads the text through the null
    // select (0x0099B31A..0x0099B32E: L"" at 0x0129A8E0 when null, else the string's start).
    const wchar_t* GetAsCharArray() const { return !PStringData ? L"" : PStringData->PeekStringData(); }   // 0x0099AF10
    long GetLength() const { return !PStringData ? 0 : PStringData->GetLength(); }
#else
    const wchar_t* GetAsCharArray() const;                     // 0x0099AF10 ?GetAsCharArray@CWideString@@QBEPBGXZ
    long GetLength() const;
#endif
    long FindCharPos(wchar_t c) const;                         // 0x0099B070 ?FindCharPos@CWideString@@QBEJG@Z
#ifdef FABLE_RETAIL_CWIDESTRING_SPRINTFDYNAMIC
    // Both PDB name sets and the neighbouring formatting methods identify this variadic member.
    void SprintfDynamic(const wchar_t* format, ...);
#endif
    void __cdecl Sprintf(const wchar_t* format, ...);          // 0x0099BA70 ?Sprintf@CWideString@@QAAXPBGZZ
    void SetLength(long length);                               // 0x0099BC50 ?SetLength@CWideString@@QAEXJ@Z
    const CWideString& operator=(const CWideString& other);
    const CWideString& operator=(const wchar_t* text);
#ifdef FABLE_RETAIL_CWIDESTRING_ASSIGNCHAR
    // Row 0x005AD571: ??4CWideString@@QAEABV0@G@Z, out of line (landed 0x0099B860).
    const CWideString& operator=(wchar_t character);             // 0x0099B860
#endif
    const CWideString& operator+=(const CWideString& other);
    const CWideString& operator+=(const wchar_t* text);
#ifdef FABLE_RETAIL_CWIDESTRING_ADDCHAR
    // Opt-in (NF220, q12401: row 0x0099C120 operator+(const CWideString&, wchar_t)): Ego_r.pdb / FableWin.pdb ??YCWideString@@QAEABV0@_W@Z (G here: wchar_t is unsigned short under the retail flags),
    // out of line in retail (landed 0x0099B9A0, called at 0x0099C15C). Declaration only.
    const CWideString& operator+=(wchar_t character);         // 0x0099B9A0
#endif
    bool operator!=(const wchar_t* other) const;
#ifdef FABLE_RETAIL_CTCQUESTCOMPLETIONUI_DESCRIPTION
    // Both original PDBs; native0099b0f0, called at005e50a2.
    bool operator!=(const CWideString& other) const;
#endif
#ifdef FABLE_RETAIL_CWIDESTRING_NE
    // Opt-in (chunk 2013, row 0x00595CC1 CFrontEndManager::ConstructFileDescription): Ego_r.pdb / FableWin.pdb
    // ??9CWideString@@QBE_NABV0@@Z, out of line in retail (0x0099B0F0; reloc_bindings.tsv, bound by the landed 0x00541E60), called
    // for `name != CUserProfileManager::Get().GetEmptySlotName()` at 0x0059612F / 0x0059628A. Declaration only.
    bool operator!=(const CWideString& other) const;           // 0x0099B0F0
#endif

    static CWideString FABLE_FASTCALL CreateFromCharString(
        const CCharString& text);
#ifdef FABLE_RETAIL_CWIDESTRING
    CCharString ToCharString() const;                          // ?ToCharString@CWideString@@QBE?AVCCharString@@XZ
#ifdef FABLE_RETAIL_CWIDESTRING_FORCEINLINE_EQ
    // Per-TU inline opt-in (chunk 784, row 0x00491AD2; like FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ): the size-optimised
    // CSubtitleRenderer TU expands both operator== bodies in place (0x00491CF6..0x00491D1B, 0x0049227C..0x004922A7) where the plain inline operators are called out of line.
    FABLE_FORCEINLINE
#endif
    bool operator==(const CWideString& other) const            // inline in retail (FableWin ??8CWideString@@QBE_NABV0@@Z)
    {
        if(PStringData==other.PStringData)return true;
        if(!PStringData || !other.PStringData)return false;
        return *PStringData==*other.PStringData;
    }
    bool Contains(const CWideString& other) const;             // ?Contains@CWideString@@QBE_NABV1@@Z
#ifdef FABLE_RETAIL_CWIDESTRING_TEXT_EQ
    // Retail opt-in (chunk 784, row 0x00491AD2): FableWin.pdb ??8CWideString@@QBE_NPBG@Z, inline in retail (0x0049227C..0x004922A7:
    // a null string compares the literal with "" through wcscmp 0x00BFE94C, else the STLport text operator== 0x00404280).
#ifdef FABLE_RETAIL_CWIDESTRING_FORCEINLINE_EQ
    FABLE_FORCEINLINE
#endif
#ifdef FABLE_RETAIL_CWIDESTRING_TEXT_EQ_HELPER
    // The default branch's shape with the helper in place of the wcscmp test (`if(...) return true; return false;`). With this
    // shape the non-null arm is placed after the next jump and jumps back to the caller's test, as retail (0x00424F4A ->
    // 0x00424EC7, 0x004262A0 -> 0x00426260); `return GFStringsAreEqual(text, L"");` keeps it in line.
    bool operator==(const wchar_t* text) const { if(!PStringData) { if(GFStringsAreEqual(text, L"")) return true; return false; } return *PStringData==text; }
#else
    bool operator==(const wchar_t* text) const { if(!PStringData) { if(wcscmp(text,L"")==0) return true; return false; } return *PStringData==text; }
#endif
#endif
#ifdef FABLE_RETAIL_CWIDESTRING_SUBSTRING
    // Retail opt-in (chunk 784, row 0x00491AD2): Ego_r.pdb / FableWin.pdb ?GetSubString@CWideString@@QBE?AV1@KK@Z, out of line;
    // (start, end) as the CSubtitleRenderer callers use it (0x00491B44: GetSubString(open + 1, close) for the text between '<' '>').
    CWideString GetSubString(unsigned long start, unsigned long end) const;   // 0x0099BDB0
#endif
    void Replace(const CWideString& search, const CWideString& replacement);   // 0x0099C1E0 ?Replace@CWideString@@QAEXABV1@0@Z (Ego_r + FableWin)
#endif
#ifdef FABLE_RETAIL_CWIDESTRING_NULLSTRING
    // Retail opt-in (chunk 1004, row 0x005C90FC ConstructWeaponDescriptionContainer): the empty wide string 0x013BCA24, Ego_r.pdb
    // `dynamic initializer for 'CWideString::NullString'' (0x0101AB90) / `dynamic atexit destructor for 'CWideString::NullString''
    // (0x0101E2E0); public static (?NullString@CWideString@@2V1@A, the spelling the landed 0x00562E10 binds). Declaration only.
    static CWideString NullString;                             // 0x013BCA24
#endif

#ifdef FABLE_RETAIL_CTCQUESTCOMPLETIONUI_DESCRIPTION
protected: // Both original TPI records: this conversion constructor is protected.
#else
private:
#endif
    explicit CWideString(const CCharString& text);
#ifndef FABLE_RETAIL_CWIDESTRING
    void UnassignString();
#endif

protected:
#if defined(FABLE_RETAIL_CWIDESTRING_MAKEOWNCOPYOFDATA) && !defined(FABLE_RETAIL_CWIDESTRING_REPLACE_PASS24)
    // Opt-in (NF220, q12401: row 0x0099C120): Ego_r.pdb / FableWin.pdb ?MakeOwnCopyOfData@CWideString@@IAEXXZ (protected; landed
    // 0x0099B560). Declaration only (FABLE_RETAIL_CWIDESTRING_REPLACE_PASS24 declares the same member).
    void MakeOwnCopyOfData();
#endif
    CWideStringData* AllocStringData(                          // 0x0099B3C0 protected (Ego_r / FableWin: IAE)
        const wchar_t* text,
        long length);

#ifdef FABLE_RETAIL_CWIDESTRING
#ifdef FABLE_RETAIL_CWIDESTRING_SPRINTF_ORIGINAL
    // Retail opt-in (Codex pass18): original02f66a00 protected SetString, debug bookkeeping excluded in native.
    void SetString(const wchar_t* string,long length=-1) {
        if(PStringData)UnassignString();
        if(!PStringData && string && *string)PStringData=AllocStringData(string,length);
    }
#endif
    static long NoStrings;
    // FableWin ?UnassignString@CWideString@@IAEXXZ (protected; inline in retail)
#if defined(FABLE_RETAIL_CWIDESTRING_SPRINTF_ORIGINAL)
    // Original02f66b70 snapshots the data pointer before changing its owner count.
    void UnassignString() {
        if(PStringData) {
            CWideStringData* data=PStringData;
            --data->NoRefs;
            if(data->NoRefs<=0)delete PStringData;
            PStringData=0;
        }
    }
#elif defined(FABLE_RETAIL_CWIDESTRING_SPRINTFDYNAMIC)
    void UnassignString() {
        CWideStringData* data=PStringData;
        if(data) { if(--data->NoRefs<=0)delete PStringData; PStringData=0; }
    }
#else
    void UnassignString() { if(PStringData) { if(--PStringData->NoRefs<=0)delete PStringData;PStringData=0; } }
#endif
    CWideStringData* PStringData;                              // +0x00 (Ego_r TPI: protected PStringData)
#else
private:
#ifdef FABLE_RETAIL_CWIDESTRING_NOSTRINGS
    // agentNF22 (Codex pass 21 NAME_FIXES land1011cr): 0x013BCA20 is the protected static instance counter
    // ?NoStrings@CWideString@@1JA (both PDBs: PUBLIC + protected-long TPI; the retail branch above declares it the same way);
    // the ctors 0x0099AED0 / 0x0099B6B0 / 0x0099B720 increment and the dtor 0x0099B510 decrements it. Replaces the extern
    // g_CWideStringInstanceCount_013BCA20 below for the default-branch rows that opt in.
protected:
    static long NoStrings;
private:
#endif

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
#ifdef FABLE_RETAIL_ANIMATION_EVENTS_LOAD_PASS29
    CCharString(class CAFile& file,long length);
    bool FastCompare(const CCharString& other) const;
#endif
#ifdef FABLE_RETAIL_STRING_MEMTRACKING_PASS26
    // Both original PDB PUBLIC/GDATA records; native string allocation category.
    static long MemTrackingID;
#endif
    CCharString();
#ifdef FABLE_RETAIL_CCHARSTRING
    // Retail opt-in (chunk 717): retail's only const char* constructor is ??0CCharString@@QAE@PBDJ@Z 0x0099EBF0 (Ego_r.pdb +
    // FableWin.pdb; no one-argument overload exists in either PDB or in Fable.exe), yet retail converts bare literals
    // implicitly through it (`push -1 ; push <lit>` at every in-place construction: braced CCharString arrays 0x0121E260 /
    // 0x0121E3A0, vector<CCharString>(18, "") 0x0121A460 which matches only through the implicit conversion, a const
    // CCharString& bound to a literal 0x0121EF70): the declaration carries the default length -1.
    CCharString(const char* text, long length = -1);           // 0x0099EBF0
#ifdef FABLE_RETAIL_CCHARSTRING_NULLSTRING
    // Opt-in (NF219, rows 0x01223EC0 / 0x0122A5E0, the NullString initializer / atexit pair): the empty string 0x013BD804, Ego_r.pdb
    // `dynamic initializer for 'CCharString::NullString'' (0x0101AAD0) / `dynamic atexit destructor ..' (0x0101E240), FableWin.pdb
    // the same pair (0x0400A4F0 / 0x04012360), Ego_d.pdb PUB32 ?NullString@CCharString@@2V1@A (public static). Declaration only.
    static CCharString NullString;                             // 0x013BD804
#endif
#ifdef FABLE_RETAIL_CCHARSTRING_RESERVE
    // Opt-in (row 0x00523CF0): Ego_r.pdb ?Reserve@CCharString@@QAEXJ@Z (0x0099EF90) and ?SetUseFastExtend@CCharString@@QAEX_N@Z (0x0099EF70).
    void Reserve(long);                                         // 0x0099EF90
    void SetUseFastExtend(bool);                                // 0x0099EF70
#endif
#ifdef FABLE_RETAIL_CCHARSTRING_SPRINTF
    // Retail opt-in (chunk 803): the varargs formatter ?Sprintf@CCharString@@QAAXPBDZZ 0x0099F1F0 (reloc_bindings; the landed
    // GFIntToCharString 0x0099F830 calls it cdecl with this pushed first, as row 0x00DDF090 does: add esp,10h after 4 pushes).
    void __cdecl Sprintf(const char* format, ...);             // 0x0099F1F0
#endif
    // Retail opt-in (Codex pass 11 consolidation): further Ego_r CCharString members used inline by retail callers.
#ifdef FABLE_RETAIL_CCHARSTRING_GETCRC_INLINE
    // Retail opt-in (chunk 892, row 0x00B21A50): Ego_r.pdb ?GetCRC@CCharString@@QBEKXZ; retail's out-of-line copy 0x00404310 is
    // `CCRC::Calc(0, GetAsCharArray(), GetLength())` (length test first, then the NullChar select, then the fastcall), and the
    // link-time-code-generated callers expand that body (0x00B21AB0..0x00B21AD0, 0x00B21BCF..0x00B21BE5).
#ifdef FABLE_RETAIL_CCHARSTRING_GETCRC_BYTELENGTH_PASS27
    // FableWin 0x0186D2A0 calls GetByteLength at 0x0186D370, which directly forwards to GetLength.
    long GetByteLength() const { return GetLength(); }
    unsigned long GetCRC() const { return CCRC::Calc(0, GetAsCharArray(), GetByteLength()); }
#else
    unsigned long GetCRC() const { return CCRC::Calc(0, GetAsCharArray(), GetLength()); }
#endif
#else
    unsigned long GetCRC() const;                              // 0x00404310 (Ego_r 0x00403C00)
#endif
    // Codex pass 13: ?SaveToFile@CCharString@@QBEXAAVCAFile@@@Z (Ego_r + FableWin), ?EqualsCaseInsensitive@CCharString@@QBE_NABV1@@Z and
    // ??MCCharString@@QBE_NABV0@@Z (FableWin; ??M bound by a landed row); all out of line in retail.
    void SaveToFile(class CAFile&) const;
    // Chunk 734 (row 0x004A0D90 CWorld::LoadQuestsFromFile): the body is inline -- retail expands it there (`_stricmp` on the two
    // GetAsCharArray results, 0x004A1028..0x004A105B; no out-of-line copy in Fable.exe or the manifest; FableWin's is the debug COMDAT).
#ifdef FABLE_RETAIL_CCHARSTRING_FORCEINLINE_EQUALSCI
    // Per-TU inline opt-in (chunk 779, row 0x00837E40 CThingTrackNode::ConstructFromLevelScript; like FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ):
    // that TU expands all four EqualsCaseInsensitive uses in place (0x00838195..0x008381C9: both GetAsCharArray ternaries, `call _stricmp`,
    // `neg ; sbb ; inc`) where the plain inline body is called out of line once the eight inlined key compares have used the budget.
    FABLE_FORCEINLINE
#endif
    bool EqualsCaseInsensitive(const CCharString& other) const { return _stricmp(GetAsCharArray(), other.GetAsCharArray()) == 0; }
#ifdef FABLE_RETAIL_CCHARSTRING_LESS_INLINE
    // (same opt-in) Ego_r.pdb ??MCCharString@@QBE_NABV0@@Z is inline in retail: _M_find 0x004B9370 tests the two PStringData
    // pointers (equal: false; this null: true; other null: false) and calls CBasicString<char>::operator< 0x00429950 on the Data
    // members (the landed 0x004B9370 model).
#ifdef FABLE_RETAIL_CCHARSTRING_FORCEINLINE_LESS
    // Per-TU inline opt-in (NF80, q10923: the /Os STLport heap / insertion-sort TUs of the CVectorMap<CCharString, ..> sorts,
    // rows 0x004DFDBB / 0x004E1C8A; like FABLE_RETAIL_CCHARSTRING_FORCEINLINE_EQUALSCI): retail expands the pointer tests in
    // place before the call to 0x00429950 (0x004DFDDB..0x004DFDE5, call at 0x004DFDEA) where under optimize("s") the plain inline body is
    // called out of line.
    FABLE_FORCEINLINE
#endif
    bool operator<(const CCharString& other) const {
        if(other.PStringData==PStringData) return false;
        if(PStringData==0) return true;
        if(other.PStringData==0) return false;
        return PStringData->Data<other.PStringData->Data;
    }
#else
    bool operator<(const CCharString&) const;
#endif
#ifdef FABLE_RETAIL_CBASICSTRING_EQ_OUTOFLINE
    // (same opt-in) retail expands CCharString::operator== in place around the CBasicString call even in the size-optimised
    // CTCVillage TU (0x0065B956..0x0065B97A), where plain inline is called out of line.
    FABLE_FORCEINLINE
#elif defined(FABLE_RETAIL_CCHARSTRING_FORCEINLINE_EQ)
    // Per-TU inline opt-in (chunk 795, row 0x005E78F0 CTCQuestCompletionUI::GlobalUpdate; like FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ):
    // that TU expands CCharString::operator==(const CCharString&) and the CBasicString<char> length-then-Compare body in place
    // (0x005E7CC2..0x005E7CF7: data pointers equal, either null, StringLength +4, `call Compare 0x00411570 ; neg ; sbb ; inc`)
    // where the plain inline body is called out of line once the function's inline budget is spent.
    FABLE_FORCEINLINE
#endif
    bool operator==(const CCharString& other) const {
        if(PStringData==other.PStringData)return true;
        if(!PStringData||!other.PStringData)return false;
        return PStringData->Data==other.PStringData->Data;
    }
#ifdef FABLE_RETAIL_PROCESS_COMMAND_LINE_PASS29
    bool operator!=(char) const;
#endif
    bool operator!=(const CCharString&) const;
    bool operator!=(const char*) const;
    const CCharString& operator+=(const char*);
    const CCharString& operator+=(const CCharString&);
#ifdef FABLE_RETAIL_CCHARSTRING_APPEND_CHAR
    // Retail opt-in (row 0x00CBFB7D RunCutsceneMacro): Ego_r.pdb / FableWin.pdb ??YCCharString@@QAEABV0@D@Z, out of line in retail at
    // 0x0099F180 (the macro parser appends one character: movsx eax,byte ptr [..] ; push eax ; call). Declaration only.
    const CCharString& operator+=(char);                       // 0x0099F180
#endif
#ifdef FABLE_RETAIL_CCHARSTRING_APPEND_COUNT
    // Retail opt-in (row 0x00CBFB7D RunCutsceneMacro, SetLightScene verb): Ego_r.pdb / FableWin.pdb ?Append@CCharString@@QAEXDK@Z, out
    // of line in retail at 0x0099F3B0 (operator+=(char) 0x0099F180 calls the same helper with count 1). Declaration only.
    void Append(char c, unsigned long count);                  // 0x0099F3B0
#endif
    // Inline in retail (no out-of-line copy is bound): ?GetLength@CCharString@@QBEJXZ (Ego_r + FableWin), ?IsEmpty@CCharString@@QBE_NXZ
    // (FableWin 0x01D34B90); 0x00522720 tests '!PStringData || !StringLength' in place.
    // GetLength: the null test as `PStringData==0 ? 0 : ...` lets VC7.1 reuse the zero CPU location (0x00E5EBD0 `test; je; mov eax,[eax+4];
    // dec eax`; the `PStringData ? ... : 0` form adds `jmp; xor eax,eax`, 1068v1064); 0x00521620 / 0x00522720 are byte-identical either way.
#ifdef FABLE_RETAIL_COMPILE_DEFINITION_PASS29
    void SetChar(long, char);
#endif
#ifdef FABLE_RETAIL_CCHARSTRING_GETCHAR
    // Retail opt-in (Codex pass 17): original public const GetChar(long), FableWin024b1fc0.
    char GetChar(long index)const{return GetAsCharArray()[index];}
#endif
    long GetLength() const { return PStringData==0 ? 0 : (long)PStringData->Data.GetLength(); }
#ifdef FABLE_RETAIL_CCHARSTRING_FORCEINLINE_ISEMPTY
    // Per-TU inline opt-in (p25O, row 0x00960120 CAIStateGroup_DeedReactionRoot::Update; like FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ):
    // that TU expands IsEmpty in place (0x00960376..0x00960381: the PStringData null test and the StringLength read, no call) where the plain
    // inline body is called out of line. __forceinline is EVIDENCED (c2 inline trace, pending/2026-11-20_c2_symbol_order/tools_inline inl.py,
    // decision 0x1078D1AA): at that site the plain body (size 29, f 1.5625) prices at cost 26 against the default limit S9 and is rejected
    // (r 0); /d2inlS25 accepts it (r 1, S21..S27 match the row) but every /d2inlS >= 21 breaks the TU-mate 0x009609B0 IsRunnable, so no
    // TU-wide setting gives retail's split. No other IsEmpty site in the TU (FableWin ai_state_group_deed_reaction_root.obj).
    FABLE_FORCEINLINE
#endif
    bool IsEmpty() const { return GetLength() == 0; }
#ifdef FABLE_RETAIL_CCHARSTRING_STARTSWITH
    // Retail opt-in (chunk 784, row 0x00491AD2): Ego_r.pdb ?StartsWith@CCharString@@QBE_NABV1@@Z, inline in retail (0x004920BE..0x004920F6:
    // the prefix's data / length from one null test, strncmp 0x00BFEAF8 on this string's GetAsCharArray, `neg ; sbb ; inc`).
    // __forceinline: the size-optimised CSubtitleRenderer TU expands it in place where a plain inline body is called out of line.
    FABLE_FORCEINLINE bool StartsWith(const CCharString& prefix) const { return strncmp(GetAsCharArray(),prefix.GetAsCharArray(),prefix.GetLength())==0; }
#endif
#if defined(FABLE_RETAIL_CCHARSTRING_STARTSWITH_ASSERT) && !defined(FABLE_RETAIL_CCHARSTRING_STARTSWITH)
    // Retail opt-in (wfC1 G1, row 0x00CBFB7D RunCutsceneMacro): StartsWith with one dead release-ASSERT arm (fable_assert.h; no bytes).
    // __forceinline is EVIDENCED: a plain inline body is 65 bytes and prices at 121..124 against the limit 15 at the verb tests (0x00CC04FA..),
    // which retail expands 185 times. The dead arm is FITTED: retail's call / expand split (GetLength called from 0x00CC0510, iterator== at
    // 0x00CC8407, vector ctors at 0x00CD0DA4 / 0x00CD0E20 / 0x00CD0F3B, every vector<CScriptThing>::size() expanded) needs 11..16 more caller
    // tuples per StartsWith expansion; FableWin's (later) debug body (21 i / 56 b) has no assert, so the true carrier is unknown. Its check
    // (Inferred, agentR w22): a non-empty prefix (an empty one matches every string; GetAsCharArray maps a null string to NullChar).
    FABLE_FORCEINLINE bool StartsWith(const CCharString& prefix) const { FABLE_RETAIL_ASSERT_ARM(prefix.PStringData != 0); return strncmp(GetAsCharArray(),prefix.GetAsCharArray(),prefix.GetLength())==0; }
#endif
#ifdef FABLE_RETAIL_CCHARSTRING_STARTSWITHCASEINSENSITIVE
    // Retail opt-in (w22 wf16 row 0x007E8590 CTCOwnedEntity::SetPOwner): FableWin.pdb ?StartsWithCaseInsensitive@CCharString@@QBE_NABV1@@Z
    // (no Ego_r / retail out-of-line copy), inline in retail: 0x007E8789..0x007E87C6 the prefix data / length from one null test, this
    // string's data, _strnicmp 0x00F3FB90.
    FABLE_FORCEINLINE bool StartsWithCaseInsensitive(const CCharString& prefix) const { return _strnicmp(GetAsCharArray(),prefix.GetAsCharArray(),prefix.GetLength())==0; }
#endif
#ifdef FABLE_RETAIL_CCHARSTRING_ENDSWITH
    // Retail opt-in (chunk 890, row 0x00498490 CProgressDisplay::DrawRetailDisplay): FableWin.pdb ?EndsWith@CCharString@@QBE_NABV1@@Z
    // (0x0187F050; no out-of-line retail copy is bound), inline in retail: 0x004993E2..0x0049945D takes the two lengths (a negative
    // difference is false), the suffix's text and this string's text from one null test each, and compares the tail with the strcmp
    // intrinsic (`sbb ; sbb ; sete`). __forceinline: Ego_r.pdb (the release build) has no out-of-line copy, and a plain inline body
    // (c2 inline cost 405 with the strcmp intrinsic) is called out of line where retail expands both 0x00498490 sites.
    FABLE_FORCEINLINE bool EndsWith(const CCharString& suffix) const
    {
        long offset = GetLength() - suffix.GetLength();
        if (offset >= 0)
            return strcmp(GetAsCharArray() + offset, suffix.GetAsCharArray()) == 0;
        return false;
    }
#endif
    // Ego_r 0x004037B0 / FableWin 0x0186C770 ?GetAsCharArray@CCharString@@QBEPBDXZ and FableWin 0x01899D30
    // ??8CCharString@@QBE_NPBD@Z, both inline in retail (0x00AE8D50 expands GetAsCharArray: `test; jne; mov eax,offset NullChar; jmp; mov eax,[eax]`;
    // the landed local models spell the same ternary). NullChar is 0x0129AAF4 (static data member per the Ego_r TPI as Codex pass 12 read it).
#if defined(FABLE_RETAIL_PASS22_CCHARSTRING_GETASCHARARRAY_ORIGINAL_IFELSE)
    // Original FableWin0186C770..0186C796 has both early-return and explicit else-end jumps.
    const char* GetAsCharArray() const { if(PStringData==0) return &NullChar; else return PStringData->Data.PeekData(); }
#elif defined(FABLE_RETAIL_CCHARSTRING_GETASCHARARRAY_SELECT)
    // Retail opt-in (chunk 848, row 0x009BC120 CPersistContext::TransferVectorOfSubComponentsLoadText): the preset-and-assign spelling
    // of the same inline accessor (the landed CPersistContext::CleanOutLine 0x009BB0D0 models it so: `text = ""; if (PStringData) text =
    // ...`). Where CleanOutLine is expanded inside 0x009BC120 (0x009BC610..0x009BC632) retail indexes the text as [ecx+eax]; the
    // ternary below gives [eax+ecx]. Same code otherwise (`mov ecx,offset NullChar ; je ; mov ecx,[eax]`).
    const char* GetAsCharArray() const { const char* text = &NullChar; if (PStringData) text = PStringData->Data.PeekData(); return text; }
#elif defined(FABLE_RETAIL_CCHARSTRING_GETASCHARARRAY_NULLRETURN)
    // Retail opt-in (chunk 890, row 0x00498490 CProgressDisplay::DrawRetailDisplay): the early-return spelling of the same inline
    // accessor. Where EndsWith expands it (0x00499411 / 0x004994E4: `mov ecx,offset NullChar ; je ; mov ecx,[esi]`) retail forms the
    // tail as `lea esi,[ecx+eax]` (text as the base, the length difference as the index); the ternary below gives [eax+ecx], and the
    // select spelling above gives [ecx+eax] at the first site only (the second site's text then outranks the offset for eax).
    const char* GetAsCharArray() const { if (PStringData==0) return &NullChar; return PStringData->Data.PeekData(); }
#else
    const char* GetAsCharArray() const { return PStringData==0 ? &NullChar : PStringData->Data.PeekData(); }
#endif
#ifdef FABLE_RETAIL_CCHARSTRING_CONTAINS
    // Retail opt-in (chunk 788, row 0x00E693F0 NScript::CV_ChapelOfEvilScript::Main): FableWin.pdb ?Contains@CCharString@@QBE_NABV1@@Z
    // (FableWin 0x018D7370; no Ego_r out-of-line copy is bound), inline in retail: 0x00E695B8..0x00E695E8 takes the pattern's text from one null test
    // (`mov ecx,offset NullChar`), then this string's, and tests strstr 0x00BFEB3A != 0 (`setne`).
    FABLE_FORCEINLINE bool Contains(const CCharString& other) const { return strstr(GetAsCharArray(),other.GetAsCharArray())!=0; }
#endif
#ifdef FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ
    // Per-TU inline opt-in (Codex pass 13, row 0x00E36D50; like FABLE_RETAIL_CBASICSTRING_EQ_OUTOFLINE): that TU expands both
    // null / non-null arms in place where the plain inline operator is called out of line.
    FABLE_FORCEINLINE
#endif
#ifdef FABLE_RETAIL_BEARDY_ORIGINAL_TEXT_COMPARISON_PASS27
    bool operator==(const char* text) const {
        if (!PStringData) {
            if (strcmp(text,"")==0) return true;
        } else {
            return PStringData->Data==text;
        }
        return false;
    }
#else
    bool operator==(const char* text) const { if(!PStringData) { if(strcmp(text,"")==0) return true; return false; } return PStringData->Data==text; }
#endif
#ifdef FABLE_RETAIL_CCHARSTRING_CHAR_EQ
    // Nested opt-in (chunk 778, row 0x00615E70 CPersistTraits<CTCInventoryMap::CUIRegionInfo>::TransferIn): FableWin.pdb
    // ??8CCharString@@QBE_ND@Z (0x020C8690; Ego_r keeps only ??9CCharString@@QBE_ND@Z, retail's out-of-line operator!= 0x0099E520).
    // Inline in retail: a null string is never equal; else {character, 0} is built on the stack and compared with the strcmp
    // intrinsic (0x006162B4: `mov byte ptr [esp+4Ch],2Ch ; mov byte ptr [esp+4Dh],0`, bytewise loop, `sbb ; sbb ; sete`).
    // __forceinline: the landed 0x0099E520 / 0x00997DA0 / 0x00997E90 local models found plain inline called out of line.
    FABLE_FORCEINLINE bool operator==(char character) const { if(PStringData!=0) { char text[2]; text[0]=character; text[1]=0; return strcmp(PStringData->Data.PeekData(),text)==0 ? true : false; } return false; }
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
#ifdef FABLE_RETAIL_CCHARSTRING_ASSIGN_CHAR
    // Retail opt-in (chunk Q25, row 0x00489D40 CPlayer::CreateCharacter): Ego_r.pdb ??4CCharString@@QAEABV0@D@Z, out of line in
    // retail at 0x0099F040 (landed; the {c, 0} buffer assign). CreateCharacter calls it with a zero-extended byte. Declaration only.
    const CCharString& operator=(char c);                       // 0x0099F040
#endif
    CWideString ToWideString() const;
#ifdef FABLE_RETAIL_CCHARSTRING_GETASLOWERCASE
    // Retail opt-in (wf13, row 0x009CCEF0): Ego_r.pdb 0x00AFA690 / FableWin.pdb 0x02F61580 ?GetAsLowerCase@CCharString@@QBE?AV1@XZ,
    // out of line in retail at 0x0099ED10. Declaration only.
    CCharString GetAsLowerCase() const;                        // 0x0099ED10
#endif
#ifdef FABLE_RETAIL_CCHARSTRING_GETASUPPERCASE
    // Retail opt-in (chunk 855, row 0x00490BE0 CSubtitleRenderer::GetGraphicFromEmbeddedTag): Ego_r.pdb 0x00AFA710 / FableWin.pdb
    // 0x02F61680 ?GetAsUpperCase@CCharString@@QBE?AV1@XZ, out of line in retail at 0x0099EE20 (it calls toupper 0x00BFED16 on every
    // byte below 0x80; its twin 0x0099ED10 calls tolower, ?GetAsLowerCase@CCharString@@QBE?AV1@XZ). Declaration only.
    CCharString GetAsUpperCase() const;                        // 0x0099EE20
#endif
#if defined(FABLE_RETAIL_CCHARSTRING_REPLACE_EGOD_PASS24)
    // Inline-budget: four complete CCharString::Replace bodies occur in 005253F0.
    FABLE_FORCEINLINE void Replace(const CCharString& What,const CCharString& With) {
        CCharString ret_str;
        char* data = (char*)GetAsCharArray();
        char* what = (char*)What.GetAsCharArray();
        char* prev = data;
        char* res = strstr(data,what);
        while (res) {
            *res=0;
            res+=strlen(what);
            ret_str+=CCharString(prev);
            ret_str+=With;
            prev=res;
            res=strstr(res,what);
        }
        ret_str+=CCharString(prev);
        *this=ret_str;
    }
#elif defined(FABLE_RETAIL_CCHARSTRING_REPLACE)
    // Retail opt-in (chunk 848, row 0x005253F0 CThingManager::LoadGameState): FableWin.pdb ?Replace@CCharString@@QAEXABV1@0@Z (0x018D9CA0,
    // public, void (const CCharString&, const CCharString&)), inline in retail: LoadGameState expands it four times (0x0052573A..0x005257FD:
    // a default-constructed result, strstr 0x00BFEB3A over the text, the match cut with `mov byte ptr [esi],0`, the search length by the
    // inline strlen loop, `+=` 0x0099F0A0 of the cut piece and of the replacement, the tail appended, then operator= 0x0099EFB0).
    FABLE_FORCEINLINE void Replace(const CCharString& search, const CCharString& replacement)
    {
        CCharString result;
        char* text = (char*)GetAsCharArray();
        const char* search_text = search.GetAsCharArray();
        char* found = strstr(text, search_text);
        while (found) {
            *found = 0;
            found += strlen(search_text);
            result += CCharString(text);
            result += replacement;
            text = found;
            found = strstr(found, search_text);
        }
        result += CCharString(text);
        *this = result;
    }
#endif

protected:
#ifdef FABLE_RETAIL_CCHARSTRING_FORMAT_PASS29
    void MakeOwnCopyOfData();
    void SetString(const char*, long);
    void SetString(const CCharString&);
public:
    static void FABLE_FASTCALL InitMemory();
protected:
#endif
#if defined(FABLE_RETAIL_CCHARSTRING_SHOP_CONSTRUCTOR26) && !defined(FABLE_RETAIL_CCHARSTRING_FORMAT_PASS29)
    void SetString(const char*, long);
public:
    static void FABLE_FASTCALL InitMemory();
protected:
#endif
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

#ifdef FABLE_RETAIL_TAVERN_SERVING_PASS29
long FABLE_FASTCALL GFCharStringToInt(const CCharString&);
#endif
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

FABLE_STATIC_ASSERT_AT(565, sizeof(CWideStringData) == 0x10);
FABLE_STATIC_ASSERT_AT(566, sizeof(CCharStringData) == 0x11);
FABLE_STATIC_ASSERT_AT(567, sizeof(CWideString) == 0x04);
FABLE_STATIC_ASSERT_AT(568, sizeof(CCharString) == 0x04);

#ifdef FABLE_RETAIL_CCHARSTRING_FORMAT_PASS29
inline void FABLE_FASTCALL CCharString::InitMemory() { ++NoStrings; }
inline CCharString::CCharString() : PStringData(0) { InitMemory(); }
inline void CCharString::UnassignString()
{
    CCharStringData* pstring_data = PStringData;
    if (pstring_data) {
        pstring_data->DecRef();
        if (pstring_data->GetRefs() <= 0)
            delete PStringData;
        PStringData = 0;
    }
}
inline CCharString::~CCharString() { UnassignString(); --NoStrings; }
inline void CCharString::SetString(const CCharString& other)
{
    if (&other != this) {

#ifdef _MSC_VER
// The native SetString and MakeOwnCopyOfData paths call UnassignString out of line.
#pragma inline_depth(0)
#endif
        if (PStringData) UnassignString();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
        if (other.PStringData) {
            PStringData = other.PStringData;
            PStringData->IncRef();
        }
    }
}
inline CCharString::CCharString(const CCharString& other) : PStringData(0) { InitMemory(); SetString(other); }
inline CCharStringData* CCharString::AllocStringData(const char* text, long length)
{
    if (length == -1) return new CCharStringData(text);
    return new CCharStringData(text, length);
}
inline void CCharString::MakeOwnCopyOfData()
{
    if (PStringData && PStringData->GetRefs() > 1) {
        CCharStringData* string_data = AllocStringData(PStringData->Data.PeekData(), -1);

#ifdef _MSC_VER
// The native SetString and MakeOwnCopyOfData paths call UnassignString out of line.
#pragma inline_depth(0)
#endif
        UnassignString();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
        PStringData = string_data;
    }
}
inline void CCharString::SetString(const char* text,long length) {
    if(PStringData) UnassignString();
    if(text && *text) PStringData=AllocStringData(text,length);
}
inline const CCharString& CCharString::operator=(const char* text) {
    SetString(text,-1);
    return *this;
}
inline const CCharString& CCharString::operator+=(char ch)
{
    if (PStringData) {
        MakeOwnCopyOfData();
        PStringData->Data.Append(ch,1);
    } else {
        char text[2] = {ch, 0};
        SetString(text, -1);
    }
    return *this;
}
inline void CCharString::Truncate(unsigned long length)
{
    if (static_cast<unsigned long>(GetLength()) > length) {
        if (length == 0) *this = "";
        else {
            MakeOwnCopyOfData();
            PStringData->Data.Truncate(length);
        }
    }
}
#include "engine/CArray.h"
#include <stdarg.h>
#include <stdio.h>
inline void FABLE_CDECL CCharString::Sprintf(const char* string, ...) {
    long buffer_char_length = strlen(string) + 1024;
    CArray<char> buffer(buffer_char_length);
    va_list arg_ptr;
    va_start(arg_ptr,string);
    #ifdef _MSC_VER
    long no_chars = _vsnprintf(&buffer[0], buffer_char_length, string, arg_ptr);
#else
    long no_chars = vsnprintf(&buffer[0], buffer_char_length, string, arg_ptr);
    if (no_chars >= buffer_char_length) no_chars = -1;
#endif
    va_end(arg_ptr);
    buffer[buffer_char_length-1]=0;
    if(no_chars<=buffer_char_length)SetString(&buffer[0],-1);
}

#endif
