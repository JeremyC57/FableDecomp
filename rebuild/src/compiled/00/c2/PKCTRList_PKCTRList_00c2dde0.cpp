// PKCTRList<CSegment>::~PKCTRList() -- the per-instance list destructor: nothing to release; the base CTypedListBase destructor's vtable
//   store (root vtable 0x012B1AB0) is all that remains (`mov dword ptr [ecx],12B1AB0h ; ret`: the list's own vptr store is dead)
// Leaf: ??1?$PKCTRList@UCSegment@@@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C2DDE0, 7 bytes. Called by PKCTRList<CSegment>'s scalar deleting destructor 0x00C2DFC0 (NF63) and
//   bound by NF52's CSegmentFile destructor funclet (0x00C2DBE0); vtable 0x012B4788 is PKCTRList<CSegment>'s.
// Identity: DESCRIPTIVE-by-library (libpk has no PDB; the trace path names libpk\PKCTRList.h): libpk/PKCTRList.h FABLE_RETAIL_PKCTRLIST_DTOR
//   declares the out-of-line virtual destructor; its body is empty, defined here for the CSegment instance.
// NF70 (round 7, land1117fu q10732): was the placeholder `T::~T` (??1T@@QAE@XZ) over a local stand-in; now the template destructor on the
//   shared header, as NF63's 0x00C2DFC0 row instantiates the class.
#define FABLE_RETAIL_CSEGMENTFILE_PASS23
#define FABLE_RETAIL_PKCTRLIST_DTOR
#include "libpk/CSegmentFile.h"

template <class T> PKCTRList<T>::~PKCTRList()
{
}
template PKCTRList<CSegment>::~PKCTRList();
