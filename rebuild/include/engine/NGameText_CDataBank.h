#ifndef FABLE_ENGINE_NGAMETEXT_CDATABANK_H
#define FABLE_ENGINE_NGAMETEXT_CDATABANK_H
// NGameText::CDataBank / CTextEntry (chunk 867, 2026-11-08): the game text bank the UI rows read titles from
// (CMainGameComponent::GetTextBank(), the counted pointer at +0x14). Ego_r.pdb + FableWin.pdb spellings, all out of line in retail:
//   ?EntryExists@CBankFile@@QBE_NK@Z                                         0x009CC190 (engine/CBankFile.h, FABLE_RETAIL_CBANKFILE_ENTRYQUERY)
//   ?GetTextInfo@CDataBank@NGameText@@QBEPBVCDataBankTextEntry@2@K@Z          0x009C7D00
//   ?GetPTextEntry@CDataBank@NGameText@@QBE?AV?$CIVCountedPointer@$$CBVCTextEntry@NGameText@@@@K_N@Z  0x009C9070 (hidden result)
//   ?GetTextBySymbol@CDataBank@NGameText@@QBE?BVCWideString@@ABVCCharString@@@Z 0x009C95E0 (hidden result)
//   ?GetText@CTextEntry@NGameText@@QBE?AVCWideString@@XZ                      0x009C7A40 (hidden result)
// RTTI: CDataBank : CBankFile (the flattened engine/CBankFile.h layout, 0x110 bytes); CTextEntry is an intrusively counted
// pointee (CIVCountedPointeeBase: vptr, IVRefCount +0x04; the rows release it through CIVCountedPointer's inline Release:
// `dec [eax+4] ; jne ; call [vtbl+4]`). Only the methods the rows call are declared; members are not modelled.
// Needs FABLE_RETAIL_DEF_CLASS_BASE (the polymorphic CIVCountedPointeeBase).
#include "fable_string.h"
#include "engine/CIVCountedPointer.h"
#ifndef FABLE_RETAIL_CBANKFILE_ENTRYQUERY
#define FABLE_RETAIL_CBANKFILE_ENTRYQUERY
#endif
#include "engine/CBankFile.h"
#ifdef FABLE_RETAIL_NGAMETEXT_DATABANK_GROUPTEXT
#ifndef FABLE_RETAIL_NGAMETEXT_GROUPENTRY
#define FABLE_RETAIL_NGAMETEXT_GROUPENTRY
#endif
#include "engine/CBankFileAsync.h"
#include "engine/CBankFileAsyncData.h"
#include "engine/CCountedPointer.h"
#include "engine/CArray.h"
#endif
#ifdef FABLE_RETAIL_NGAMETEXT_TEXTENTRY_READ_PASS26
class CDataInputStream;
#endif
namespace NGameText {
#ifdef FABLE_RETAIL_NGAMETEXT_CONSTRUCTSTRING_LONGS
// Both originals and native009c7b70/009c7b80: references, namespace, fastcall.
bool FABLE_FASTCALL ConstructString(CWideString&,long);
bool FABLE_FASTCALL ConstructString(CWideString&,long,long);
#ifdef FABLE_RETAIL_NGAMETEXT_CONSTRUCTSTRING_LONGS3
// Nested opt-in (w22 agent2012, row 0x006DE280): Ego_r.pdb ?ConstructString@NGameText@@YI_NAAVCWideString@@JJJ@Z (Ego_r 0x00B159A0, after
// the J / JJ overloads 0x00B15970 / 0x00B15980 = retail 0x009C7B70 / 0x009C7B80), retail 0x009C7BA0 (`ret 8`, forwards a, b, c, -1).
bool FABLE_FASTCALL ConstructString(CWideString&,long,long,long);
#endif
#endif
#ifdef FABLE_RETAIL_NGAMETEXT_CONSTRUCTSTRING_VECTOR
// Row 0x005AD571: ?ConstructString@NGameText@@YI_NAAVCWideString@@ABV?$vector@VCWideString@@...@std@@@Z (fastcall), out of line.
bool FABLE_FASTCALL ConstructString(CWideString& text, const std::vector<CWideString>& args);   // 0x009C7C20
#endif
class CDataBankTextEntry;
#if defined(FABLE_RETAIL_NGAMETEXT_GETPTEXTENTRY) && defined(FABLE_RETAIL_NGAMETEXT_DATABANK_GROUPTEXT)
// Nested opt-in (p25N, row 0x009C9170 CDataBank::Validate and its TU-mates GetPTextEntry 0x009C9070 / BeginLoadingText 0x009C8630 /
// GetTextInfo 0x009C7D00 / IsLoaded 0x009C7BE0 / GetText 0x009C7A40): CTextEntry is a CResource like CGroupEntry (ghidra_out/rtti_bases.tsv:
// .?AVCTextEntry@NGameText@@ : CResource@0, CIVCountedPointeeBase@0;
// struct_layouts_egor.tsv CTextEntry, 72 bytes: 28 bytes of base, SpeechBank +28, Narrator +32, SoundName +36, Text +40 (CWideString),
// Tags +44 (CArray<CTextTag>, 16 bytes in Ego_r, 12 in retail), PFileData, Owner). Retail offsets: Text +0x28 (GetText 0x009C7A40
// `add ecx,28h`; Validate 0x009C9217 `lea edx,[ebx+28h]`), PFileData +0x38 (GetPTextEntry 0x009C9070, IsLoaded 0x009C7BE0), Owner +0x40.
// The text entry's TouchResource (FableWin.pdb ?TouchResource@CResource@@QAEXXZ, inline; 0x009C9119..0x009C914C) is CResource's.
// Needs FABLE_RETAIL_NGAMETEXT_GETPGROUPENTRY.
}
#include "engine/CResource.h"
namespace NGameText {
#ifdef FABLE_RETAIL_NGAMETEXT_TEXTENTRY_READ_PASS26
class CTextTag {public:long Pos;CCharString Name;};
FABLE_STATIC_ASSERT(sizeof(CTextTag)==8);
#endif
class CTextEntry : public CResource {
public:
#ifdef FABLE_RETAIL_NGAMETEXT_TEXTENTRY_READ_PASS26
    // Both original PDBs; native009c8de0 and Sizeof009c7d90.
    void ReadFromDataStream(CDataInputStream& info_stream);
    unsigned long Sizeof() const;
#endif
    CWideString GetText() const;                                                   // 0x009C7A40
    CCharString SpeechBank;                                                        // +0x1C
    CCharString Narrator;                                                          // +0x20
    CCharString SoundName;                                                         // +0x24
    CWideString Text;                                                              // +0x28
#ifdef FABLE_RETAIL_NGAMETEXT_TEXTENTRY_READ_PASS26
    CArray<CTextTag> Tags;
#else
    unsigned char Tags[0x38 - 0x2c];                                               // +0x2C CArray<CTextTag>
#endif
    CCountedPointer<CBankFileAsyncData> PFileData;                                 // +0x38
    CDataBankTextEntry* Owner;                                                     // +0x40
};
FABLE_STATIC_ASSERT_AT(68, offsetof(CTextEntry, Text) == 0x28);
FABLE_STATIC_ASSERT_AT(69, offsetof(CTextEntry, PFileData) == 0x38);
FABLE_STATIC_ASSERT_AT(70, sizeof(CTextEntry) == 0x44);
#else
class CTextEntry : public CIVCountedPointeeBase {
public:
    CWideString GetText() const;                                                   // 0x009C7A40
#ifdef FABLE_RETAIL_NGAMETEXT_DATABANK_GROUPTEXT
    // Opt-in (row 0x009C9280): the pending bank read at +0x38 (landed IsLoaded 0x009C7BE0 / GetPTextEntry 0x009C9070 read it there;
    // Ego_r PFileData, a CCountedPointer<CBankFileAsyncData> whose first word is the object).
    unsigned char _pad_0x08[0x38 - 0x08];
    CCountedPointer<CBankFileAsyncData> PFileData;                                 // +0x38
#endif
#if defined(FABLE_RETAIL_NGAMETEXT_TEXTENTRY_SPEECH) && !defined(FABLE_RETAIL_NGAMETEXT_DATABANK_GROUPTEXT)
    // Opt-in (agentNF54, row 0x0065C781 CTCVillage::FrameUpdate): the speech members the row reads (Ego_r.pdb struct layout
    // NGameText::CTextEntry: SpeechBank +28 (0x1C), Narrator +32, SoundName +36 (0x24), all CCharString; retail reads +0x1C and +0x24).
    unsigned char _pad_0x08[0x1c - 0x08];
    CCharString SpeechBank;                                                        // +0x1C
    CCharString Narrator;                                                          // +0x20
    CCharString SoundName;                                                         // +0x24
#endif
};
#endif
#ifdef FABLE_RETAIL_NGAMETEXT_DATABANK_GROUPTEXT
// Opt-in (row 0x009C9280): Ego_r.pdb CDataBankEntry (an empty tag, 1 byte) and CDataBankTextEntry (8 bytes: PText +0, NarratorIndex +4);
// inline ?GetNarratorIndex@CDataBankTextEntry@NGameText@@QBEJXZ is named only by FableWin.pdb (fablewin_pdb_names.tsv /
// FableWin.exe.signatures.tsv; not in egor_pdb_names.tsv); ?IsLoaded@CDataBankTextEntry@NGameText@@QBE_NXZ at 0x009C7BE0 (both PDBs).
class CDataBankEntry { };
class CDataBankTextEntry : public CDataBankEntry {
public:
    CIVCountedPointer<CTextEntry> PText;                                           // +0x00
    long NarratorIndex;                                                            // +0x04
    long GetNarratorIndex() const { return NarratorIndex; }
    bool IsLoaded() const;                                                         // 0x009C7BE0
#ifdef FABLE_RETAIL_NGAMETEXT_GETPTEXTENTRY
    // Nested opt-in (p25N, row 0x009C9170): Ego_r.pdb 0x00B16820 / 0x00B17870 private const ?BeginLoading@CDataBankTextEntry@NGameText@@ABEXKAAVCDataBank@2@@Z
    // (0x009C8340) and ?BuildText@CDataBankTextEntry@NGameText@@ABEXXZ (0x009C8FB0), as CDataBankGroupEntry's pair; CDataBank calls them.
    friend class CDataBank;
private:
    void BeginLoading(unsigned long id, CDataBank& bank) const;                    // 0x009C8340
    void BuildText() const;                                                        // 0x009C8FB0
public:
#endif
};
#ifdef FABLE_RETAIL_NGAMETEXT_GETPGROUPENTRY
// Nested opt-in (agentNF69, row 0x009C8890 GetPGroupEntry): Ego_r.pdb CDataBankGroupEntry (4 bytes: PGroup, a CIVCountedPointer<CGroupEntry>)
// and its private const members ?BeginLoading@CDataBankGroupEntry@NGameText@@ABEXKAAVCDataBank@2@@Z (0x009C80F0) and
// ?BuildGroup@CDataBankGroupEntry@NGameText@@ABEXXZ (0x009C8700), both PDBs; CDataBank calls them (a friend). Declarations only.
class CGroupEntry;
class CDataBank;
class CDataBankGroupEntry : public CDataBankEntry {
    friend class CDataBank;
public:
    CIVCountedPointer<CGroupEntry> PGroup;                                         // +0x00
private:
    void BeginLoading(unsigned long id, CDataBank& bank) const;                    // 0x009C80F0
    void BuildGroup() const;                                                       // 0x009C8700
};
#else
class CDataBankGroupEntry : public CDataBankEntry { };
#endif
#endif
#ifdef FABLE_RETAIL_NGAMETEXT_GROUPENTRY
// Nested opt-in (chunk 1011, row 0x005EB180 CTCTavernGame::SetStateAfterInstructions): the text group a bank entry id names.
// Ego_r.pdb ?GetPGroupEntry@CDataBank@NGameText@@QBE?AV?$CIVCountedPointer@$$CBVCGroupEntry@NGameText@@@@K_N@Z (out of line,
// 0x009C8890; rebuild/oracles/reloc_bindings.tsv), RTTI CGroupEntry : CIVCountedPointeeBase. The group's text ids are the
// CArray<long> at +0x1C (landed 0x0048F6BD / 0x00639BA0 read it there; the member name is the landed rows' spelling, the PDB
// layout lists no members for this class). Declaration only for the bank; no layout change to CDataBank.
// Correction (row 0x009C9280): struct_layouts_egor.tsv does list CGroupEntry (56 bytes): Entries +28 (0x1C) CArray<long>, then
// PFileData +44 and Owner +52, which confirms the member name.
}
#include "engine/CArray.h"
namespace NGameText {
#ifdef FABLE_RETAIL_NGAMETEXT_GETPGROUPENTRY
// Nested opt-in (agentNF69, row 0x009C8890): CGroupEntry is a class with public base CResource at offset 0 (Ego_d LF_BCLASS; Ego_r lays out
// its first 28 bytes as padding = sizeof(CResource), then Entries +28, PFileData +44 (CCountedPointer<CBankFileAsyncData>), Owner +52;
// ?UnloadResource@CGroupEntry@NGameText@@UAE_NXZ overrides CResource's slot; docs/journal/2026-11/EGOD_FINDINGS.md section 5). Retail's
// CArray is 12 bytes, so PFileData is +0x28 (0x009C8890 reads [group+28h]) and Owner +0x30. Needs engine/CResource.h's polymorphic class
// (FABLE_RETAIL_DEF_CLASS_BASE).
}
#include "engine/CResource.h"
namespace NGameText {
class CGroupEntry : public CResource {
public:
    virtual bool UnloadResource();                                                 // Ego_r ?UnloadResource@CGroupEntry@NGameText@@UAE_NXZ
#ifdef FABLE_RETAIL_NGAMETEXT_CGROUPENTRY_CTOR
    // NF72 opt-in (row 0x009C80F0 CDataBankGroupEntry::BeginLoading): Ego_r.pdb 0x00B15D40 / FableWin.pdb 0x02F75940
    // ??0CGroupEntry@NGameText@@QAE@AAV?$CCountedPointer@VCBankFileAsyncData@@@@PBVCDataBankGroupEntry@1@@Z, out of line in retail
    // (0x009C8060). Declaration only: no layout change.
    CGroupEntry(CCountedPointer<CBankFileAsyncData>& file_data, const CDataBankGroupEntry* owner);   // 0x009C8060
#endif
#ifdef FABLE_RETAIL_NGAMETEXT_CGROUPENTRY_SIZEOF
    // Nested opt-in (NF80, q10931: row 0x009C8060, the constructor's SetResourceSize(Sizeof())): Ego_r.pdb 0x00B15C70 / FableWin.pdb
    // 0x02F75E70 ?Sizeof@CGroupEntry@NGameText@@QBEKXZ, inline in retail: the object, the id array's storage and the file data
    // through GFPSizeof (0x009C8096..0x009C80B7: `lea edi,[edi*4+34h]`, the PFileData test, `call 0x009D5620 ; add eax,8`, the -8
    // folded into `lea ecx,[eax+edi-8]`). Needs FABLE_RETAIL_CARRAY_SIZEOF, FABLE_RETAIL_GFPISNULL_CCOUNTEDPOINTER (engine/GFSizeof.h)
    // and FABLE_RETAIL_CBANKFILEASYNCDATA_SIZEOF.
    // The array part is a local summed first (retail holds it in edi across the PFileData test and tail-duplicates the caller's
    // SetResourceSize into both arms); one expression lets VC7.1 evaluate the GFPSizeof call first (104 v 130 bytes).
    unsigned long Sizeof() const {
        unsigned long size = sizeof(*this) + Entries.Sizeof() - sizeof(Entries);
        return size + GFPSizeof(PFileData) - sizeof(PFileData);
    }
#endif
    CArray<long> Entries;                                                          // +0x1C
    CCountedPointer<CBankFileAsyncData> PFileData;                                 // +0x28
    CDataBankGroupEntry* Owner;                                                    // +0x30
#else
class CGroupEntry : public CIVCountedPointeeBase {
public:
    unsigned char _pad_0x08[0x1c - 0x08];
    CArray<long> Entries;                                                          // +0x1C
#endif
#ifdef FABLE_RETAIL_NGAMETEXT_DATABANK_GROUPTEXT
    const CArray<long>& GetTextEntries() const { return Entries; }                 // FableWin.pdb ?GetTextEntries@CGroupEntry@NGameText@@QBEABV?$CArray@J@@XZ (inline; not in Ego_r)
#endif
};
#endif
#ifdef FABLE_RETAIL_NGAMETEXT_DATABANK_GROUPTEXT
// Opt-in (row 0x009C9280): CDataBank : CBankFileAsync (0x164) with the members this row reads at their retail offsets (Ego_r names):
// Entries +0x1E0 (landed GetTextInfo 0x009C7D00), Narrators +0x204 (landed ConvertNarratorNameToIndex 0x009C7EF0), Seed +0x210.
#ifdef FABLE_RETAIL_NGAMETEXT_GETPGROUPENTRY
// Nested opt-in (agentNF69, row 0x009C8890): the second base, RTTI .?AVCDataBank@NGameText@@ : CBankFileAsync@0, CResourceBank@356 (0x164,
// rtti_bases.tsv; engine/CResourceBank.h, 0x7C bytes: ResourceList +0x168, LoadingQueue +0x1A4 here), which fills the padding before
// Entries +0x1E0; and the out-of-line ?BeginLoadingGroup@CDataBank@NGameText@@QBEXK@Z (0x009C8220, Ego_r.pdb + FableWin.pdb).
}
#include "engine/CResourceBank.h"
namespace NGameText {
class CDataBank : public CBankFileAsync, public CResourceBank {
public:
    void BeginLoadingGroup(unsigned long id) const;                                // 0x009C8220
#else
class CDataBank : public CBankFileAsync {
public:
#ifdef FABLE_RETAIL_NGAMETEXT_CDATABANK_CTOR_DECL
    // b24c4 opt-in (row 0x00402510 GFMain): Ego_r.pdb ??0CDataBank@NGameText@@QAE@XZ (out of line at 0x009C85A0;
    // GFMain `new` + call), ??1CDataBank@NGameText@@UAE@XZ and the protected override ?GetDataFormatVersion@CDataBank@NGameText@@MAEKXZ
    // of the pure CBankFile slot (so the class is concrete). Declarations only.
    CDataBank();                                                                   // 0x009C85A0
    virtual ~CDataBank();
protected:
    virtual unsigned long GetDataFormatVersion();
public:
#endif
    unsigned char _pad_0x164[0x1e0 - 0x164];                                       // CResourceBank base and the bank state
#endif
    CArray<CDataBankEntry*> Entries;                                               // +0x1E0
    unsigned char _pad_0x1ec[0x204 - 0x1ec];                                       // TextEntryBuffer / GroupEntryBuffer
    CArray<CCharString> Narrators;                                                 // +0x204
    mutable unsigned long Seed;                                                    // +0x210
    const CDataBankGroupEntry* GetGroupInfo(unsigned long id) const { if (GetEntryDataType(id) == 1) return static_cast<const CDataBankGroupEntry*>(Entries[id]); return 0; }
    void BeginLoadingText(unsigned long textId) const;                             // 0x009C8630
    const CTextEntry* GetTextEntryFromGroup(unsigned long groupId, const CCharString& narrator, bool& found, long* lastId, bool allowPending) const;
#else
class CDataBank : public CBankFile {
public:
#endif
#if defined(FABLE_RETAIL_CTCTALK_VOICED_GROUP_PASS24) && !defined(FABLE_RETAIL_NGAMETEXT_DATABANK_GROUPTEXT)
    const CTextEntry* GetTextEntryFromGroup(unsigned long groupId, const CCharString& narrator, bool& found, long* lastId, bool allowPending) const;
#endif
#ifdef FABLE_RETAIL_NGAMETEXT_GROUPENTRY
    CIVCountedPointer<const CGroupEntry> GetPGroupEntry(unsigned long groupId, bool a) const;   // 0x009C8890
#endif
    const CDataBankTextEntry* GetTextInfo(unsigned long textId) const;             // 0x009C7D00
    CIVCountedPointer<const CTextEntry> GetPTextEntry(unsigned long textId, bool a) const;   // 0x009C9070
    const CWideString GetTextBySymbol(const CCharString& symbol) const;            // 0x009C95E0
#ifdef FABLE_RETAIL_NGAMETEXT_GETPTEXTENTRY
    void Validate();                                                               // 0x009C9170 ?Validate@CDataBank@NGameText@@QAEXXZ (both PDBs)
#endif
};
#ifdef FABLE_RETAIL_NGAMETEXT_CONSTRUCTSTRING
// Retail opt-in (chunk 890, row 0x00498490 CProgressDisplay::DrawRetailDisplay): Ego_r.pdb 0x00B156F0
// ?ConstructString@NGameText@@YI_NAAVCWideString@@ABV2@@Z (__fastcall: ecx = the text, edx = the argument; substitutes the argument
// into the text's parameter tag), out of line in retail at 0x009C7980. Declaration only.
bool __fastcall ConstructString(CWideString& text, const CWideString& argument);   // 0x009C7980
#endif
}
#endif // FABLE_ENGINE_NGAMETEXT_CDATABANK_H
