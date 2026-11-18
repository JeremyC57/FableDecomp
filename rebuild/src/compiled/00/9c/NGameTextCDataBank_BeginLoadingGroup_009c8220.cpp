// NGameText::CDataBank::BeginLoadingGroup(unsigned long id) const -- start the asynchronous load of a group entry: when the bank entry is a
// group (GetGroupInfo: data type 1 and the entry record exists), the entry's BeginLoading with this bank
// Leaf: ?BeginLoadingGroup@CDataBank@NGameText@@QBEXK@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C8220, length 42 bytes.
// Callees: CBankFile::GetEntryDataType(unsigned long) const @ 0x009CC210 (the inline GetGroupInfo), NGameText::CDataBankGroupEntry::BeginLoading
//   (unsigned long, CDataBank&) const @ 0x009C80F0.
// NF72 (round 7, q10908 / q10906): Ego_r.pdb 0x00B16000 + FableWin.pdb 0x02F71030 ?BeginLoadingGroup@CDataBank@NGameText@@QBEXK@Z; the ported
//   GetPGroupEntry 0x009C8890 (NF69) binds it and defines this function's text in its own TU (graded RELOCATION_MATCH against 0x009C8220 there).
//   Re-authored with that text and macro block on engine/NGameText_CDataBank.h (GETPGROUPENTRY); the local CDataBank / CDataBankGroupEntry /
//   CArray models are gone, and the callee 0x009C80F0 now binds the PDB private const ?BeginLoading@CDataBankGroupEntry@NGameText@@ABEXKAAVCDataBank@2@@Z.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_DELETES
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_CONVERT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_NGAMETEXT_DATABANK_GROUPTEXT
#define FABLE_RETAIL_NGAMETEXT_GETPGROUPENTRY
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CRESOURCE_TOUCHRESOURCE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CRESOURCELIST_MAKEFREESPACE
#define FABLE_RETAIL_C3DANIMATIONINFO_BUILD
#include "fable_string.h"
#include "engine/NGameText_CDataBank.h"

namespace NGameText {
void CDataBank::BeginLoadingGroup(unsigned long id) const
{
    const CDataBankGroupEntry* entry = GetGroupInfo(id);
    if (entry)
        entry->BeginLoading(id, const_cast<CDataBank&>(*this));
}
}
