// std::_Vector_base<const CThingCreatureBase*, std::allocator<const CThingCreatureBase*> >::~_Vector_base() -- STLport vector storage destructor: free the element block when there is one
// Leaf: ??1?$_Vector_base@PBVCThingCreatureBase@@V?$allocator@PBVCThingCreatureBase@@@std@@@std@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006CE220, 14 bytes, thiscall, size TU (`pop ecx` after free). Callee: free 0x00BFEA14 (MSVCR71).
// Identity (NF391 trace, NF396 re-check): its only references are the two calls 0x006CE1D8 / 0x006CE1F4 (`lea ecx,[esp+50h] ; call`)
//   at the two returns of CGroundNavigatorExternalInterface::IsMovementBlockedAt (FableWin.pdb; retail 0x006CDDF0, a body the catalog
//   does not split), on its local vector<CThingCreatureBase const*> (FableWin: the same function destroys one local of that type at
//   each exit; the landed vector<const CThingCreatureBase*>::reserve 0x006CE230 sits right after this body). The inlined STLport
//   ~vector (`_Destroy(_M_start, _M_finish)`, empty for a pointer) leaves only the base destructor call; a ~vector COMDAT in a size TU
//   is the 5-byte tail jump instead (0x004DF54F -> 0x004DF554). Both PDBs are Dinkumware builds (no _Vector_base), so the spelling is the
//   one the repo STLport emits (EXTRA_RULES 2026-11-20 descriptive-by-library).
// NF396: was the free stand-in OnReadFinished(S*) (catalog FreeMember0, manifest CActiveFile::OnReadFinished, propagated). Model: the
//   shared stlport/fable_containers.h _Vector_base; the explicit instantiation of the destructor alone emits it as its own pick-any
//   COMDAT (the 0x004D9C5D form without the constructors a whole-class instantiation would add).
#pragma optimize("s", on)
#include "stlport/fable_containers.h"
class CThingCreatureBase;

template std::_Vector_base<const CThingCreatureBase*, std::allocator<const CThingCreatureBase*> >::~_Vector_base();
