// std::_Vector_base<const CSteeringAgent*, std::allocator<const CSteeringAgent*> >::~_Vector_base() -- STLport vector storage destructor: free the element block when there is one
// Leaf: ??1?$_Vector_base@PBVCSteeringAgent@@V?$allocator@PBVCSteeringAgent@@@std@@@std@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DF554, 14 bytes, thiscall, size TU (`pop ecx` after free). Callee: free 0x00BFEA14 (MSVCR71).
// Identity (NF391 trace, NF396 re-check): its only reference is the 5-byte `jmp 0x004DF554` at 0x004DF54F, the out-of-line
//   ~vector<const CSteeringAgent*> (the element destroy is empty for a pointer, so ~vector is a tail jump into its base's destructor);
//   0x004DF54F's only caller is CObstacleNavigatorExternalInterface's scalar deleting destructor 0x004DF507 (vftable 0x01241D04) on the
//   member at +8 = Ego_r.pdb CObstacleNavigatorExternalInterface::DummyAgentList, vector<CSteeringAgent const*> (+8 in both PDBs);
//   FableWin's ~CObstacleNavigatorExternalInterface (tc_register.obj) destroys that member through ~vector<CSteeringAgent const*>. Both PDBs are
//   Dinkumware builds (no _Vector_base), so the spelling is the one the repo STLport emits (EXTRA_RULES 2026-11-20 descriptive-by-library).
// NF396: was the free stand-in OnReadFinished(S*) (catalog FreeMember0, manifest CActiveFile::OnReadFinished, propagated). Model: the
//   shared stlport/fable_containers.h _Vector_base; the explicit instantiation of the destructor alone emits it as its own pick-any
//   COMDAT (the 0x004D9C5D form without the constructors a whole-class instantiation would add).
#pragma optimize("s", on)
#include "stlport/fable_containers.h"
class CSteeringAgent;

template std::_Vector_base<const CSteeringAgent*, std::allocator<const CSteeringAgent*> >::~_Vector_base();
