// NGameText::CTextEntry::GetText() const -- returns a copy of the entry's wide text (the CWideString Text member at +0x28) by value (hidden result pointer, out-of-line CWideString copy constructor)
// Leaf: ?GetText@CTextEntry@NGameText@@QBE?AVCWideString@@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C7A40, length 22 bytes (to the `ret 4` ending at 0x009C7A56; the old line said 20).
// Identity: both PDBs carry the Leaf symbol (Ego_r.pdb 0x00B157B0); callers such as CItem::GetItemDescription 0x005BD8F3 call it on the
//   pointee of CDataBank::GetPTextEntry 0x009C9070. Callee: CWideString::CWideString(const CWideString&) 0x0099B720.
// Re-port (p25N, 2026-11-20): onto engine/NGameText_CDataBank.h (overlay opt-in FABLE_RETAIL_NGAMETEXT_GETPTEXTENTRY: CTextEntry : CResource,
//   struct_layouts_egor.tsv members, Text at +0x28), for the same-TU copy in 0x009C9170 CDataBank::Validate, which expands it. Was a local
//   model with a descriptive member name.
// Text: Ego_d.pdb (databank.cpp, line-table file id 432) has a debug build of this function (lines 877..904, two text_copy scopes and a
//   static seed); retail is only its last statement, `return Text;` (Ego_d line 903), the rest is compiled out of the release build.
// Define block: the databank.cpp rows' common block (0x009C9170 Validate and its TU-mates), so every same-TU copy compiles from one text.
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
CWideString CTextEntry::GetText() const
{
    return Text;
}
}
