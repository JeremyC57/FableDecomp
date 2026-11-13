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
class CTextEntry : public CIVCountedPointeeBase {
public:
    CWideString GetText() const;                                                   // 0x009C7A40
#ifdef FABLE_RETAIL_NGAMETEXT_DATABANK_GROUPTEXT
    // Opt-in (row 0x009C9280): the pending bank read at +0x38 (landed IsLoaded 0x009C7BE0 / GetPTextEntry 0x009C9070 read it there;
    // Ego_r PFileData, a CCountedPointer<CBankFileAsyncData> whose first word is the object).
    unsigned char _pad_0x08[0x38 - 0x08];
    CCountedPointer<CBankFileAsyncData> PFileData;                                 // +0x38
#endif
};
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
};
class CDataBankGroupEntry : public CDataBankEntry { };
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
class CGroupEntry : public CIVCountedPointeeBase {
public:
    unsigned char _pad_0x08[0x1c - 0x08];
    CArray<long> Entries;                                                          // +0x1C
#ifdef FABLE_RETAIL_NGAMETEXT_DATABANK_GROUPTEXT
    const CArray<long>& GetTextEntries() const { return Entries; }                 // FableWin.pdb ?GetTextEntries@CGroupEntry@NGameText@@QBEABV?$CArray@J@@XZ (inline; not in Ego_r)
#endif
};
#endif
#ifdef FABLE_RETAIL_NGAMETEXT_DATABANK_GROUPTEXT
// Opt-in (row 0x009C9280): CDataBank : CBankFileAsync (0x164) with the members this row reads at their retail offsets (Ego_r names):
// Entries +0x1E0 (landed GetTextInfo 0x009C7D00), Narrators +0x204 (landed ConvertNarratorNameToIndex 0x009C7EF0), Seed +0x210.
class CDataBank : public CBankFileAsync {
public:
    unsigned char _pad_0x164[0x1e0 - 0x164];                                       // CResourceBank base and the bank state
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
#ifdef FABLE_RETAIL_NGAMETEXT_GROUPENTRY
    CIVCountedPointer<const CGroupEntry> GetPGroupEntry(unsigned long groupId, bool a) const;   // 0x009C8890
#endif
    const CDataBankTextEntry* GetTextInfo(unsigned long textId) const;             // 0x009C7D00
    CIVCountedPointer<const CTextEntry> GetPTextEntry(unsigned long textId, bool a) const;   // 0x009C9070
    const CWideString GetTextBySymbol(const CCharString& symbol) const;            // 0x009C95E0
};
#ifdef FABLE_RETAIL_NGAMETEXT_CONSTRUCTSTRING
// Retail opt-in (chunk 890, row 0x00498490 CProgressDisplay::DrawRetailDisplay): Ego_r.pdb 0x00B156F0
// ?ConstructString@NGameText@@YI_NAAVCWideString@@ABV2@@Z (__fastcall: ecx = the text, edx = the argument; substitutes the argument
// into the text's parameter tag), out of line in retail at 0x009C7980. Declaration only.
bool __fastcall ConstructString(CWideString& text, const CWideString& argument);   // 0x009C7980
#endif
}
#endif // FABLE_ENGINE_NGAMETEXT_CDATABANK_H
