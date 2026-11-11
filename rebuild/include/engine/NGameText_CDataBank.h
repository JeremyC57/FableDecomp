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
namespace NGameText {
#ifdef FABLE_RETAIL_NGAMETEXT_CONSTRUCTSTRING_LONGS
// Both originals and native009c7b70/009c7b80: references, namespace, fastcall.
bool FABLE_FASTCALL ConstructString(CWideString&,long);
bool FABLE_FASTCALL ConstructString(CWideString&,long,long);
#endif
class CDataBankTextEntry;
class CTextEntry : public CIVCountedPointeeBase {
public:
    CWideString GetText() const;                                                   // 0x009C7A40
};
#ifdef FABLE_RETAIL_NGAMETEXT_GROUPENTRY
// Nested opt-in (chunk 1011, row 0x005EB180 CTCTavernGame::SetStateAfterInstructions): the text group a bank entry id names.
// Ego_r.pdb ?GetPGroupEntry@CDataBank@NGameText@@QBE?AV?$CIVCountedPointer@$$CBVCGroupEntry@NGameText@@@@K_N@Z (out of line,
// 0x009C8890; rebuild/oracles/reloc_bindings.tsv), RTTI CGroupEntry : CIVCountedPointeeBase. The group's text ids are the
// CArray<long> at +0x1C (landed 0x0048F6BD / 0x00639BA0 read it there; the member name is the landed rows' spelling, the PDB
// layout lists no members for this class). Declaration only for the bank; no layout change to CDataBank.
}
#include "engine/CArray.h"
namespace NGameText {
class CGroupEntry : public CIVCountedPointeeBase {
public:
    unsigned char _pad_0x08[0x1c - 0x08];
    CArray<long> Entries;                                                          // +0x1C
};
#endif
class CDataBank : public CBankFile {
public:
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
