// CSegmentList::`scalar deleting destructor'(unsigned int flags) -- run ~CSegmentList, free the block when bit 0 of the flags is set, return this
// Leaf: ??_GCSegmentList@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C2E080, 30 bytes (`ret 4`).
// Identity (NF56, round 7, queue q10442): the retail vftable 0x012B47D0 (= ??_7CSegmentList@@6B@, libpk/CSegmentFile.h; NF44 e104) holds
//   0x00C2E080 in its destructor slot, and the body calls 0x00C2E030, the landed out-of-line ??1CSegmentList@@UAE@XZ
//   (FABLE_RETAIL_CSEGMENTLIST_DTOR_OUTOFLINE), then operator delete on flags & 1: the scalar deleting destructor VC7.1 emits for the
//   virtual destructor (no array branch). Descriptive libpk names (no libpk PDB): one address, one name with the header and the dtor row.
//   Was the stand-in ?VecDel@T@@QAEPAXI@Z over a local struct T (catalog CVtable_012b47d0::`scalar deleting destructor').
// VC7.1 emits the vftable, and with it this scalar deleting destructor, where the inline constructor is compiled, so the row forces it with a
//   Kill_ helper (the 0x00C17800 pattern); the helper is not part of the retail body.
#define FABLE_RETAIL_CSEGMENTFILE_PASS23
#define FABLE_RETAIL_CSEGMENTLIST_DTOR_OUTOFLINE
#include "libpk/CSegmentFile.h"

void Kill_00c2e080()
{
    CSegmentList list;
}
