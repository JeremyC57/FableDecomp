// Leaf: ??0CThingNoiseDef@@QAE@XZ
// CThingNoiseDef::CThingNoiseDef() -- inline default constructor, emitted out of line: CThingBaseDef() out of line, this class's
// vftable, then the Noise member (+0x70) through CTCNoiseDef's inline constructor (CSubDefClassBase(), then the CTCNoiseDef vftable)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00455EF0, 36 bytes, thiscall, `ret`.
// Identity (two sources): Ego_r.pdb and FableWin.pdb ??0CThingNoiseDef@@QAE@XZ; retail's only reference to it is the tail `jmp` of Alloc
//   0x0045C5C1 (both PDBs ?Alloc@CThingNoiseDef@@SIPAVCDefClassBase@@XZ, `push 0A4h` = sizeof). Its stores are the CThingNoiseDef vftable
//   0x0123388C (RTTI .?AVCThingNoiseDef@@) and, at Noise, the CTCNoiseDef vftable 0x01231E9C; its calls ??0CThingBaseDef@@QAE@XZ 0x006772D0 and
//   ??0CSubDefClassBase@@QAE@XZ 0x0044C0C0 (both PDBs).
// Option S unit (owner ruling, advisor decision 2026-11-24; unit NF379): this function is emitted by its retail TU, CThingNoiseDef's
//   def TU (Ego_r.pdb / FableWin.pdb definition_manager.obj), whose one source is row 0x0045C5F6, included below with no code of its own
//   (the EH-funclet / 0x00B2AB10 shape); that object's copy grades against this address on the Leaf above. Before the unit this row carried
//   the same text (NF363 option A), a second strong ?Alloc@CThingNoiseDef; it was a strong ctor on a local stand-in model before that.
#include "../src/compiled/00/45/CThingNoiseDef_CThingNoiseDef_0045c5f6.cpp"
