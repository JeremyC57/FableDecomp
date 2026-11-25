// CEngineSceneEnumerator::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: ~CEngineSceneEnumerator, then operator
//   delete when bit 0 of flags is set; returns this
// Leaf: ??_GCEngineSceneEnumerator@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B31DC0, 30 bytes, thiscall, `ret 4`; vftable 0x012A12E8 slot 0.
// Identity (NF424, S1 local model): retail RTTI .?AV of the vftable plus the FableWin / Ego_r name sets (round-1 histogram).
//   Was ?ScalarDeletingDestructor@CEngineSceneEnumerator@@QAEPAXI@Z.
// TU: VC7.1 emits ??_GCEngineSceneEnumerator@@UAEPAXI@Z as a pick-any COMDAT beside the vtable, only in a TU whose code stores the vptr. 0x00B31D80
//   ??0CEngineSceneEnumerator@@QAE@XZ stores vftable 0x012A12E8, so this function is produced by that translation unit, included below with no code of its own
//   (the nf251 0x00B2AB10 / Option S thin-row pattern, NF422 ctor-owned units).
#include "../src/compiled/00/b3/CEngineSceneEnumerator_CEngineSceneEnumerator_00b31d80.cpp"
