// NGameText::CDataBank::GetTextInfo(unsigned long index) const -- the text-bank entry record for a bank index, or null when the entry's data type is not the text type (0)
// Leaf: ?GetTextInfo@CDataBank@NGameText@@QBEPBVCDataBankTextEntry@2@K@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C7D00, length 39 bytes (to the `ret 4` ending at 0x009C7D27; the old line said 37).
// Identity: both PDBs carry the Leaf symbol. Callee: CBankFile::GetEntryDataType(unsigned long) 0x009CC210.
// Re-port (p25N, 2026-11-20): onto engine/NGameText_CDataBank.h (CDataBank : CBankFileAsync, CResourceBank; Entries +0x1E0 as
//   CArray<CDataBankEntry*>), for the same-TU copies in 0x009C8630 / 0x009C9070 / 0x009C9170, which expand it (Validate keeps only its
//   GetEntryDataType call, the result unused). Was a local model of CBankFile / CDataBank with a padded layout.
// Text: Ego_d.pdb (databank.cpp, file id 432) lines 140 / 142 / 144 = 16 / 22 / 2 bytes at /Od, this text the same; parameter `index`.
//   Ego_d lines 137..139 (370 / 300 / 318 bytes) are debug checks with no retail code. Ego_d names the data types in the nested enum
//   NGameText::CDataBank::EDataType (TYPE_TEXT = 0, TYPE_GROUP = 1, TYPE_NARRATOR_LIST = 2); that is one source only, so the literal stays,
//   as in the shared header's GetGroupInfo.
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
const CDataBankTextEntry* CDataBank::GetTextInfo(unsigned long index) const
{
    if (GetEntryDataType(index) == 0)
    {
        return static_cast<const CDataBankTextEntry*>(Entries[index]);
    }
    return 0;
}
}
