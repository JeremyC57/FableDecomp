// NGameText::CDataBankTextEntry::IsLoaded() const -- true when the text entry object exists and either has no pending asynchronous bank read or that read has finished
// Leaf: ?IsLoaded@CDataBankTextEntry@NGameText@@QBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C7BE0, length 30 bytes.
// Identity: Ego_r.pdb 0x00B159E0 / FableWin.pdb carry the Leaf symbol (namespace NGameText; the landed file defined the global
//   ?IsLoaded@CDataBankTextEntry@@QBE_NXZ, NAME_FIXES.tsv); landed 0x009C9280 GetTextEntryFromGroup already calls the namespaced spelling.
//   Callee: CBankFileAsyncData::IsFinished() 0x009D54E0.
// Re-port (p25N, 2026-11-20): onto engine/NGameText_CDataBank.h (CDataBankTextEntry's PText; overlay opt-in FABLE_RETAIL_NGAMETEXT_GETPTEXTENTRY
//   for CTextEntry's PFileData at +0x38), for the same-TU copy in 0x009C9070 GetPTextEntry, which expands it. Was a local model.
// Text: Ego_d.pdb (databank.cpp, file id 432) lines 767 / 769 / 771 / 775 = 12 / 57 / 4 / 2 bytes at /Od; this text 15 / 57 / 4 / 2
//   (the outer test through CIVCountedPointer::IsNull).
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
}
