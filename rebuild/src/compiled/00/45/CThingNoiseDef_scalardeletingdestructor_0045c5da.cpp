// Leaf: ??_GCThingNoiseDef@@UAEPAXI@Z
// CThingNoiseDef::`scalar deleting destructor'(unsigned int) -- the implicit ~CThingNoiseDef (0x0045C5F6), then operator delete when bit 0
// of the flags is set; returns this
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0045C5DA, 28 bytes, thiscall, `ret 4`.
// Identity (two sources): Ego_r.pdb and FableWin.pdb ??_GCThingNoiseDef@@UAEPAXI@Z; slot 0 of the CThingNoiseDef vftable 0x0123388C
//   (ghidra_out/labels_rtti_port_dtors.tsv); a scalar body (no array loop), so ??_G (deleting-destructor ruling). VC7.1 emits it only from
//   the TU that emits the vftable, the constructor's TU: C++ cannot name ??_G, so no in-place respell of the old free stand-in exists.
// Option S unit (owner ruling, advisor decision 2026-11-24; unit NF379): this function is emitted by its retail TU, CThingNoiseDef's
//   def TU (Ego_r.pdb / FableWin.pdb definition_manager.obj), whose one source is row 0x0045C5F6, included below with no code of its own
//   (the EH-funclet / 0x00B2AB10 shape); that object's copy grades against this address on the Leaf above. Before the unit this row carried
//   the same text (NF363 option A), a second strong ?Alloc@CThingNoiseDef.
#include "../src/compiled/00/45/CThingNoiseDef_CThingNoiseDef_0045c5f6.cpp"
