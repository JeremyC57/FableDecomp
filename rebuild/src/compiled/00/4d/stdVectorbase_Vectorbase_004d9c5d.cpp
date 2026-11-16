// std::_Vector_base<std::pair<long,CCharString>, std::allocator<std::pair<long,CCharString> > >::~_Vector_base() -- STLport 4.6: free the element block when one was allocated (a folded COMDAT: 14 bytes, `if (_M_start) free(_M_start)`)
// Leaf: ??1?$_Vector_base@U?$pair@JVCCharString@@@std@@V?$allocator@U?$pair@JVCCharString@@@std@@@2@@std@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D9C5D, 14 bytes, size-optimised (`pop ecx` after the call). Callee: free 0x00BFEA14 (MSVCR71).
// Name: descriptive-by-library (both PDBs are Dinkumware builds). The three call sites of this address, all in
//   NScript::CQ_BountyHuntScript::WatchForBoasts 0x00D17A80 (its only callers, callers.py), are the ~_Vector_base relocations of that
//   byte-matching object, which binds exactly this spelling; the element type pair<long,CCharString> is the one that row's vector holds.
//   The body is the plain STLport _Vector_base destructor (no type-specific code), so the folded address takes the name its only caller
//   binds.
// NF41 (2026-11-16): was a descriptive placeholder `CFirstMemberOwner_004d9c5d::ReleaseFirstMember_004d9c5d`; now one explicit
//   instantiation of the shared stlport/fable_containers.h _Vector_base, compiled /Os as before.
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#include "stlport/fable_containers.h"
#include "fable_string.h"
template class std::_Vector_base<std::pair<long, CCharString>, std::allocator<std::pair<long, CCharString> > >;
