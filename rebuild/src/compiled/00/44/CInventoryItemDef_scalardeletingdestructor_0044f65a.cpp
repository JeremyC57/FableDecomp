// CInventoryItemDef::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: ~CInventoryItemDef, then operator
//   delete when bit 0 of flags is set; returns this
// Leaf: ??_GCInventoryItemDef@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0044F65A, 34 bytes, thiscall, `ret 4`; vftable 0x01231DBC slot 0.
// Identity (NF424, S5): retail RTTI .?AV of the vftable plus the FableWin / Ego_r name sets (round-1 histogram).
//   Was ?ScalarDeletingDestructor@CInventoryItemDef@@QAEPAXI@Z.
// TU: VC7.1 emits ??_GCInventoryItemDef@@UAEPAXI@Z as a pick-any COMDAT beside the vtable, only in a TU whose code stores the vptr. 0x0044C108
//   ??0CInventoryItemDef@@QAE@XZ stores vftable 0x01231DBC, so this function is produced by that translation unit, included below with no code of its own
//   (the nf251 0x00B2AB10 / Option S thin-row pattern, NF422 ctor-owned units).
#include "../src/compiled/00/44/CInventoryItemDef_CInventoryItemDef_0044c108.cpp"
