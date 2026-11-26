// CInputProcessDebugControls::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: the implicit ~CInputProcessDebugControls inlined (PCreature's ~CBaseIntelligentPointer 0x00A01C10, then ~CAInputProcess 0x00A0D3D0), then operator
//   delete when bit 0 of flags is set; returns this
// Leaf: ??_GCInputProcessDebugControls@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0048B8D0, 40 bytes, thiscall, `ret 4`; vftable 0x01237F9C slot 0.
// Identity (NF477 thin-row port, nf424 pattern; NF424 S1 for the 60 siblings): retail RTTI .?AV of the vftable plus the FableWin / Ego_r name sets (round-1 histogram).
//   Was ?CInputProcessDebugControls_CompositeScalarDeletingDestructor@@YIPAXPAXHI@Z.
// NF477 (2026-11-26): the misspelt free stand-in respelt to the compiler-generated ??_G; the owner TU's copy is byte- and relocation-equal to
//   retail at HEAD (ceq). TU: VC7.1 emits ??_GCInputProcessDebugControls@@UAEPAXI@Z as a pick-any COMDAT beside the vtable, only in a TU whose code stores the vptr. 0x00488D10
//   ?InitInterfaces@CPlayer@@AAEXXZ stores vftable 0x01237F9C, so this function is produced by that translation unit, included below with no code of its own
//   (the nf251 0x00B2AB10 / Option S thin-row pattern, NF422 ctor-owned units).
#include "../src/compiled/00/48/CPlayer_InitInterfaces_00488d10.cpp"
