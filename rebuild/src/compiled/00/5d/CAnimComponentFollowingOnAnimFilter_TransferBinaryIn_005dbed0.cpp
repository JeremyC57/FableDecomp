// CAnimComponentFollowingOnAnimFilter::TransferBinaryIn(CMemoryDataInputStream& stream) -- read the follow-on animation filter name (virtual)
// Leaf: ?TransferBinaryIn@CAnimComponentFollowingOnAnimFilter@@UAEXAAVCMemoryDataInputStream@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x005DBED0, 8 bytes: `add ecx,8 ; jmp 0x0044FC00` (NextFilter.TransferBinaryIn(stream), tail call).
// Identity (two sources): slot 6 of CAnimComponentFollowingOnAnimFilter's vtable 0x012557D8 (rtti-vtables.tsv; the slot the landed
//   CAnimComponentSetCreatureMode / DelayTime / TransitionInTime TransferBinaryIn rows occupy in their vtables), and Ego_r.pdb / FableWin.pdb
//   ?TransferBinaryIn@CAnimComponentFollowingOnAnimFilter@@UAEXAAVCMemoryDataInputStream@@@Z; the class's only member is the CDefString
//   NextFilter at +0x08 (Ego_r struct_layouts), and the jump target 0x0044FC00 is CDefString::TransferBinaryIn.
// Callees: ?TransferBinaryIn@CDefString@@QAEXAAVCDataInputStream@@@Z 0x0044FC00 (the PDB spelling; reloc_bindings also carries three
//   invented pointer / CMemoryDataInputStream variants of it, NAME_FIXES.tsv).
// Header: engine/CAnimComponentFollowingOnAnimFilter.h (FABLE_RETAIL_CANIMCOMPONENTFOLLOWINGONANIMFILTER), engine/CDefString.h
//   (FABLE_RETAIL_CDEFSTRING_TRANSFER), engine/CMemoryDataInputStream.h (FABLE_RETAIL_CMEMORYDATAINPUTSTREAM).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#define FABLE_RETAIL_CDEFSTRING_TRANSFER
#define FABLE_RETAIL_CANIMCOMPONENTFOLLOWINGONANIMFILTER
#include "fable_string.h"
#include "engine/CMemoryDataInputStream.h"
#include "engine/CAnimComponentFollowingOnAnimFilter.h"
void CAnimComponentFollowingOnAnimFilter::TransferBinaryIn(CMemoryDataInputStream& stream)
{
    NextFilter.TransferBinaryIn(stream);
}