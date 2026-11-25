// std::vector<const CSteeringAgent*, std::allocator<const CSteeringAgent*> >::~vector() -- STLport vector destructor: destroying the
//   pointer elements is empty, so the body is a tail jump into the base's out-of-line destructor (~_Vector_base 0x004DF554)
// Leaf: ??1?$vector@PBVCSteeringAgent@@V?$allocator@PBVCSteeringAgent@@@std@@@std@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DF54F, 5 bytes: `jmp 0x004DF554` (size-optimised TU).
// Identity (NF391 trace, NF396 / NF399 re-check): its only reference is the call at 0x004DF513 in CObstacleNavigatorExternalInterface's
//   scalar deleting destructor 0x004DF507 (vftable 0x01241D04) on the member at +8, Ego_r.pdb CObstacleNavigatorExternalInterface::
//   DummyAgentList, vector<CSteeringAgent const*>; FableWin's ~CObstacleNavigatorExternalInterface (tc_register.obj) destroys that
//   member through ~vector<CSteeringAgent const*>. Its jump target 0x004DF554 is ~_Vector_base<const CSteeringAgent*> (landed nf396_b1).
//   Both PDBs are Dinkumware builds (no _Vector_base), so the spelling is the one the repo STLport emits (descriptive-by-library).
// NF399: was the free forwarder stand-in _Forward calling ForwardTarget_004df554 (catalog _global::TailJmp_OnReadFinished_004df54f,
//   manifest CActiveFile::OnReadFinished, propagated), and the table bound the stand-in ??1CMember_004df54f@@QAE@XZ here (witness
//   0x004DF507, respelt in the same landing). Model: explicit instantiation of the shared stlport/fable_containers.h destructor, with
//   ~_Vector_base declared only (FABLE_RETAIL_STLP_VECTOR_BASE_DTOR_DECL_ONLY): with the inline base body VC7.1 expands it (14 bytes).
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_VECTOR_BASE_DTOR_DECL_ONLY
#include "stlport/fable_containers.h"
class CSteeringAgent;

template std::vector<const CSteeringAgent*, std::allocator<const CSteeringAgent*> >::~vector();
