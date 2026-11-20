// NGameText::CDataBank::GetPTextEntry(unsigned long index, bool wait) const -- the text entry a bank index names: flush the pending bank reads; when the entry is loaded (or the caller waits) build it (a first load: begin loading, build, trim the resource cache; or a finished read), touch it in its resource list and return a counted pointer to it; otherwise start loading it and return null
// Leaf: ?GetPTextEntry@CDataBank@NGameText@@QBE?AV?$CIVCountedPointer@$$CBVCTextEntry@NGameText@@@@K_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C9070, 245 bytes, thiscall (`ret 0Ch`: hidden result, index, wait).
// Identity: Ego_r.pdb 0x00B17AB0 / FableWin.pdb carry the Leaf symbol. Callees (both PDBs' spellings): CBankFile::GetEntryDataType 0x009CC210,
//   CBankFileAsync::AsyncUpdatePending 0x009D68B0, CBankFileAsyncData::IsFinished 0x009D54E0, CDataBank::BeginLoadingText 0x009C8630,
//   CDataBankTextEntry::BeginLoading 0x009C8340 / BuildText 0x009C8FB0 (private const, ABEX..), CResourceList::MakeFreeSpace 0x009FC4F0,
//   CResource::ListRemove 0x009FBFB0 / ListInsert 0x009FBF90.
// Re-port (p25N, 2026-11-20): onto engine/NGameText_CDataBank.h (overlay opt-in FABLE_RETAIL_NGAMETEXT_GETPTEXTENTRY: CTextEntry : CResource,
//   ghidra_out/rtti_bases.tsv; PFileData +0x38) and Ego_d.pdb's text, for the same-TU copy in 0x009C9170 Validate. Was a local model of
//   CResource / CResourceList / CIVCountedPointer / CBankFile / CDataBank under renaming macros (an "independent pure C++03 candidate").
// Same-TU bodies (verbatim copies): IsLoaded 0x009C7BE0, GetTextInfo 0x009C7D00 (both expanded: 0x009C9077.., 0x009C9097..) and
//   BeginLoadingText 0x009C8630 (called on the not-loaded path, expanded on the first-load path 0x009C90D5..0x009C90F0). Each one
//   declaration-only: DIFFER(232v245) / (236v245) / (221v245).
// Text oracle: Ego_d.pdb (databank.cpp, file id 432) locals this, index, wait, text_info; 16 line records, lines 162..187. This text gives
//   the same records in the same order: 15 / 10 / 8 / 23 / 15 / 12 / 8 / 16 / 27 / 8 / 15 / 17 / 12 / 11 bytes at /Od against Ego_d's
//   15 / 10 / 8 / 27 / 15 / 12 / 8 / 25 / 22 / 8 / 15 / 26 / 12 / 20 (Ego_d is a VC10 build: the counted-pointer and base-adjust lines
//   differ). The 27-byte line 167 is the IsLoaded() call (the expanded expression is one 94-byte line); the not-loaded BeginLoadingText
//   is the fall-through after the loaded block (line 183), and there is no `else` record.
// Define block: the databank.cpp rows' common block (0x009C9170 Validate and its TU-mates). Release spelling: the TU-mate 0x009C9280's
//   FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_DELETES + FABLE_RETAIL_CIVCOUNTEDPOINTER_RELEASE_MEMBER (this row expands no release).
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
// Fidelity: same-TU strong definition of 0x009C7BE0 (retail TU-mate)
bool CDataBankTextEntry::IsLoaded() const
{
    if (!PText.IsNull())
    {
        if (PText->PFileData.IsNull() || PText->PFileData->IsFinished())
        {
            return true;
        }
    }
    return false;
}

// Fidelity: same-TU strong definition of 0x009C7D00 (retail TU-mate)
const CDataBankTextEntry* CDataBank::GetTextInfo(unsigned long index) const
{
    if (GetEntryDataType(index) == 0)
    {
        return static_cast<const CDataBankTextEntry*>(Entries[index]);
    }
    return 0;
}

// Fidelity: same-TU strong definition of 0x009C8630 (retail TU-mate)
void CDataBank::BeginLoadingText(unsigned long entry_index) const
{
    const CDataBankTextEntry* info = GetTextInfo(entry_index);
    if (info)
    {
        info->BeginLoading(entry_index, const_cast<CDataBank&>(*this));
    }
}

CIVCountedPointer<const CTextEntry> CDataBank::GetPTextEntry(unsigned long index, bool wait) const
{
    const CDataBankTextEntry* text_info = GetTextInfo(index);
    if (text_info)
    {
        const_cast<CDataBank*>(this)->AsyncUpdatePending();
        if (text_info->IsLoaded() || wait)
        {
            if (text_info->PText.IsNull())
            {
                BeginLoadingText(index);
                text_info->BuildText();
                const_cast<CDataBank*>(this)->ResourceList.MakeFreeSpace(0);
            }
            else if (!text_info->PText->PFileData.IsNull())
            {
                text_info->BuildText();
            }
            text_info->PText->TouchResource();
            return text_info->PText;
        }
        BeginLoadingText(index);
    }
    return CIVCountedPointer<const CTextEntry>();
}
}
