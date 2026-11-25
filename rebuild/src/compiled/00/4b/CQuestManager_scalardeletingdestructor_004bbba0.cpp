// CQuestManager::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: ~CQuestManager, then operator
//   delete when bit 0 of flags is set; returns this
// Leaf: ??_GCQuestManager@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004BBBA0, 30 bytes, thiscall, `ret 4`; vftable 0x01239464 slot 0.
// Identity (NF424, S2 G2): retail RTTI .?AV of the vftable plus the FableWin / Ego_r name sets (round-1 histogram).
//   Was ?CQuestManager_scalar_deleting_destructor@@YIPAXPAVCQuestManager@@HI@Z.
// TU: VC7.1 emits ??_GCQuestManager@@UAEPAXI@Z as a pick-any COMDAT beside the vtable, only in a TU whose code stores the vptr. 0x004B4590
//   ??0CQuestManager@@QAE@AAVCWorld@@AAVCScriptInfoManager@@AAVCGameScriptInterface@@@Z stores vftable 0x01239464, so this function is produced by that translation unit, included below with no code of its own
//   (the nf251 0x00B2AB10 / Option S thin-row pattern, NF422 ctor-owned units).
#include "../src/compiled/00/4b/CQuestManager_CQuestManager_004b4590.cpp"
