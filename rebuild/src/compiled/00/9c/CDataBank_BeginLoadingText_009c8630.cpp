// NGameText::CDataBank::BeginLoadingText(unsigned long entry_index) const -- start the asynchronous load of a text entry: only when the bank entry's data type is the text type (0) and the entry record exists
// Leaf: ?BeginLoadingText@CDataBank@NGameText@@QBEXK@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C8630, length 43 bytes (to the `ret 4` ending at 0x009C865B; the old line said 42).
// Identity: Ego_r.pdb 0x00B16C80 / FableWin.pdb 0x02F70FE0 ?BeginLoadingText@CDataBank@NGameText@@QBEXK@Z (const, namespace NGameText; the
//   landed file defined the global non-const ?BeginLoadingText@CDataBank@@QAEXK@Z, NAME_FIXES.tsv). Callees: CBankFile::GetEntryDataType
//   0x009CC210 (through GetTextInfo, expanded) and the private const ?BeginLoading@CDataBankTextEntry@NGameText@@ABEXKAAVCDataBank@2@@Z
//   0x009C8340 (Ego_r.pdb 0x00B16820; ecx = the record, pushes: the bank, then the index).
// Re-port (p25N, 2026-11-20): onto engine/NGameText_CDataBank.h (overlay opt-in FABLE_RETAIL_NGAMETEXT_GETPTEXTENTRY declares BeginLoading /
//   BuildText as CDataBankGroupEntry's pair is declared), as the landed sibling BeginLoadingGroup 0x009C8220 is written. Was a local model.
// Same-TU body: GetTextInfo, a verbatim copy of the row 0x009C7D00; without it the call is not expanded: DIFFER(32v43).
// Text: Ego_d.pdb (databank.cpp, file id 432) locals entry_index / info; lines 406 / 407 / 409 = 15 / 6 / 16 bytes at /Od, this text the same.
// Define block: the databank.cpp rows' common block (0x009C9170 Validate and its TU-mates).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_DELETES
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_CONVERT
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RELEASE_MEMBER
#define FABLE_RETAIL_NGAMETEXT_DATABANK_GROUPTEXT
#define FABLE_RETAIL_NGAMETEXT_GETPGROUPENTRY
#define FABLE_RETAIL_NGAMETEXT_GETPTEXTENTRY
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CRESOURCE_TOUCHRESOURCE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CRESOURCELIST_MAKEFREESPACE
#define FABLE_RETAIL_C3DANIMATIONINFO_BUILD
#include "fable_string.h"
#include "engine/NGameText_CDataBank.h"

namespace NGameText {
// Fidelity: same-TU strong definition of 0x009C7D00 (retail TU-mate)
const CDataBankTextEntry* CDataBank::GetTextInfo(unsigned long index) const
{
    if (GetEntryDataType(index) == 0)
    {
        return static_cast<const CDataBankTextEntry*>(Entries[index]);
    }
    return 0;
}

void CDataBank::BeginLoadingText(unsigned long entry_index) const
{
    const CDataBankTextEntry* info = GetTextInfo(entry_index);
    if (info)
    {
        info->BeginLoading(entry_index, const_cast<CDataBank&>(*this));
    }
}
}
