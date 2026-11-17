// PKCTRList<CSegment>::`scalar deleting destructor'(unsigned int) -- the compiler-generated deleting destructor of libpk's typed list front over CSegment (vtable 0x012B4788 slot 4): runs ~PKCTRList<CSegment> (0x00C2DDE0) and frees the object when bit 0 of the flags is set
// Leaf: ??_G?$PKCTRList@UCSegment@@@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C2DFC0, 30 bytes. Default flags (no /EHsc).
// NF63 (round 7): was a hand-written fastcall stand-in (CLandscapeBackgroundPatch_vector_deleting_destructor over a local one-word
// CLandscapeBackgroundPatch, binding the free CLandscapeBackgroundPatch_dtor_00c2dde0 for 0x00C2DDE0; both catalog labels propagated).
// It is the deleting destructor VC7.1 generates for the shared libpk/PKCTRList.h template instance the libpk/CSegmentFile.h
// CSegmentList derives from (NF52's CSegmentFile destructor 0x00C2DBE0 and its funclet 0x01213BDE bind ~PKCTRList<CSegment> at
// 0x00C2DDE0 as ??1?$PKCTRList@UCSegment@@@@UAE@XZ); the explicit instantiation emits the class's vtable and with it this function; the destructor is declared out of line
// (overlay FABLE_RETAIL_PKCTRLIST_DTOR), as retail calls it.
#define FABLE_RETAIL_CSEGMENTFILE_PASS23
#define FABLE_RETAIL_PKCTRLIST_DTOR
#include "libpk/CSegmentFile.h"
template class PKCTRList<CSegment>;
