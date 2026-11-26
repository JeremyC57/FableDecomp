// CAIStateGroup_FlyingThingBase::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: the implicit
//   ~CAIStateGroup_FlyingThingBase (inlined: its four float members need no destruction, so it is the base ~CAIStateGroupBase 0x008FD288), then
//   operator delete when bit 0 of flags is set; returns this.
// Leaf: ??_GCAIStateGroup_FlyingThingBase@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00913490, 30 bytes, thiscall, `ret 4` (/O2: `add esp,4` after the delete call). Identity (NF472, 0x008FD288 stand-in
//   binder): slot 0 of retail vftable 0x0128F9BC (RTTI .?AVCAIStateGroup_FlyingThingBase@@ : CAIStateGroupBase : CBaseClass), the vftable the
//   constructor 0x009126C0 stores; ??_G is VC7.1's scalar deleting destructor (no vector form in retail, GOTCHAS NF424). Ego_r.pdb / FableWin.pdb
//   type records flag ~CAIStateGroup_FlyingThingBase compiler-generated (engine/CAIStateGroup_FlyingThingBase.h: four floats +0x14..+0x20).
//   Was ?ScalarDeletingDestructor@CAIStateGroup_FlyingThingBase@@QAEPAXI@Z on a local stand-in struct whose call bound 0x008FD288 as
//   ?Dtor@CAIStateGroup_FlyingThingBase@@QAEXXZ.
// TU: VC7.1 emits ??_GCAIStateGroup_FlyingThingBase@@UAEPAXI@Z as a pick-any COMDAT beside the vtable, only in a TU whose code stores the vptr.
//   0x009126C0 ??0CAIStateGroup_FlyingThingBase@@QAE@XZ (same /O2 retail TU, the class's own .cpp: ctor 0x009126C0, members 0x00912700..0x009130D0,
//   then this COMDAT) stores vftable 0x0128F9BC and emits this function byte- and relocation-equal to retail (call ??1CAIStateGroupBase@@UAE@XZ =
//   0x008FD288, operator delete), so it is produced by that translation unit, included below with no code of its own (the nf251 0x00B2AB10 /
//   Option S thin-row pattern, NF422 / nf424 ctor-owned units).
#include "../src/compiled/00/91/CAIStateGroupFlyingThingBase_CAIStateGroupFlyingThingBase_009126c0.cpp"
