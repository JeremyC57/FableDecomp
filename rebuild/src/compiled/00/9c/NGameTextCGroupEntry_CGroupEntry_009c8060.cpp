// NGameText::CGroupEntry::CGroupEntry(CCountedPointer<CBankFileAsyncData>& file_data, const CDataBankGroupEntry* owner) -- a text-group resource entry: CResource base, an empty id array, a shared reference to the group's file data and its owning bank entry; registers its memory size
// Leaf: ??0CGroupEntry@NGameText@@QAE@AAV?$CCountedPointer@VCBankFileAsyncData@@@@PBVCDataBankGroupEntry@1@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C8060, length 128 bytes (padded to 144). Ego_r.pdb 0x00B15D40 / FableWin.pdb 0x02F75940
//   (engine/NGameText_CDataBank.h FABLE_RETAIL_NGAMETEXT_CGROUPENTRY_CTOR declares it; the landed 0x009C80F0 BeginLoading binds it).
// Callees: CResource::CResource @ 0x009FBEC0, CBankFileAsyncData::Sizeof @ 0x009D5620 (Ego_r.pdb ?Sizeof@CBankFileAsyncData@@QBEKXZ: the
//   old row's "CGroup" was the file data), CResource::SetResourceSize @ 0x009FBF20.
// Vtable: NGameText::CGroupEntry 0x0129B478 (rtti-vtables.tsv).
// Shape: SetResourceSize(Sizeof()) with CGroupEntry::Sizeof (Ego_r.pdb / FableWin.pdb ?Sizeof@CGroupEntry@NGameText@@QBEKXZ) inline:
//   sizeof(*this) + the CArray<long>'s reserved storage + GFPSizeof(PFileData) (FableWin ??@10585CBV?@..; the
//   pointer test, Data->Sizeof() + 8 or 8) - sizeof(PFileData), the -8 folded into the final `lea`.
// NF80 (q10931): moved off the local CResource / vector / CCountedPointer / NGameText::CGroup models onto engine/NGameText_CDataBank.h
//   (FABLE_RETAIL_NGAMETEXT_GETPGROUPENTRY + _CGROUPENTRY_CTOR; the overlay's _CGROUPENTRY_SIZEOF), engine/CArray.h (overlay
//   FABLE_RETAIL_CARRAY_SIZEOF), engine/GFSizeof.h (overlay FABLE_RETAIL_GFPISNULL_CCOUNTEDPOINTER) and engine/CBankFileAsyncData.h
//   (FABLE_RETAIL_CBANKFILEASYNCDATA_SIZEOF); preamble as the landed 0x009C80F0.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_CONVERT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CARRAY_SIZEOF
#define FABLE_RETAIL_NGAMETEXT_DATABANK_GROUPTEXT
#define FABLE_RETAIL_NGAMETEXT_GETPGROUPENTRY
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CRESOURCE_TOUCHRESOURCE
#define FABLE_RETAIL_CRESOURCE_SETRESOURCESIZE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CRESOURCELIST_MAKEFREESPACE
#define FABLE_RETAIL_C3DANIMATIONINFO_BUILD
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RESET_ADOPT
#define FABLE_RETAIL_CBANKFILEASYNC_GETASYNCENTRY
#define FABLE_RETAIL_CBANKFILE_ENTRYDATASIZE
#define FABLE_RETAIL_CBANKFILEASYNCDATA_SIZEOF
#define FABLE_RETAIL_GFPISNULL_CCOUNTEDPOINTER
#define FABLE_RETAIL_NGAMETEXT_CGROUPENTRY_CTOR
#define FABLE_RETAIL_NGAMETEXT_CGROUPENTRY_SIZEOF
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/GFSizeof.h"
#include "engine/NGameText_CDataBank.h"

namespace NGameText {
CGroupEntry::CGroupEntry(CCountedPointer<CBankFileAsyncData>& file_data, const CDataBankGroupEntry* owner)
    : PFileData(file_data), Owner(const_cast<CDataBankGroupEntry*>(owner))
{
    SetResourceSize(Sizeof());
}
}
