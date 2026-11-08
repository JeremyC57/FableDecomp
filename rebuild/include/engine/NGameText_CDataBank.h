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
class CDataBankTextEntry;
class CTextEntry : public CIVCountedPointeeBase {
public:
    CWideString GetText() const;                                                   // 0x009C7A40
};
class CDataBank : public CBankFile {
public:
    const CDataBankTextEntry* GetTextInfo(unsigned long textId) const;             // 0x009C7D00
    CIVCountedPointer<const CTextEntry> GetPTextEntry(unsigned long textId, bool a) const;   // 0x009C9070
    const CWideString GetTextBySymbol(const CCharString& symbol) const;            // 0x009C95E0
};
}
#endif // FABLE_ENGINE_NGAMETEXT_CDATABANK_H
