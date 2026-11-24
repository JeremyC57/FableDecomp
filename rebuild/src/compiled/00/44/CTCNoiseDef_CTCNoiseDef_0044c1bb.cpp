// Leaf: ??0CTCNoiseDef@@QAE@XZ
// CTCNoiseDef::CTCNoiseDef() -- inline default constructor, emitted out of line: CSubDefClassBase(), then this class's vftable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0044C1BB, 18 bytes, thiscall, `ret`.
// Identity (two sources): FableWin.pdb ??0CTCNoiseDef@@QAE@XZ (Ego_r.pdb expands it); the body calls ??0CSubDefClassBase@@QAE@XZ 0x0044C0C0
//   and stores the CTCNoiseDef vftable 0x01231E9C (RTTI .?AVCTCNoiseDef@@ : CSubDefClassBase). Retail references 0x0044C1BB nowhere (no
//   call, jmp or dword; NF359 xscan): it is the unreferenced out-of-line COMDAT of the inline constructor, which this TU emits too (the only
//   relocation naming it is a .debug$F record; NF359 evidence orphan_ctcnoisedef_ctor.txt). Retail's copy sits with no padding before
//   GetSizeofClass 0x0044C1CD (alignment 1, a size-optimised COMDAT).
// Option S unit (owner ruling, advisor decision 2026-11-24; unit NF379): this function is emitted by its retail TU, CThingNoiseDef's
//   def TU (Ego_r.pdb / FableWin.pdb definition_manager.obj), whose one source is row 0x0045C5F6, included below with no code of its own
//   (the EH-funclet / 0x00B2AB10 shape); that object's copy grades against this address on the Leaf above. Before the unit this row carried
//   the same text (NF363 option A), a second strong ?Alloc@CThingNoiseDef.
#include "../src/compiled/00/45/CThingNoiseDef_CThingNoiseDef_0045c5f6.cpp"
