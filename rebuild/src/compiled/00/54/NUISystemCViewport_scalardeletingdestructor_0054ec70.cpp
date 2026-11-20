// NUISystem::CViewport::`scalar deleting destructor'(unsigned int flags) -- run the out-of-line CViewport destructor, then free the
//   object when flag bit 0 is set; returns this
// Leaf: ??_GCViewport@NUISystem@@UAEPAXI@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x0054EC70, 30 bytes, thiscall `ret 4`: call 0x0054EAC0, `test byte [esp+8],1`, operator delete
//   0x00BFE9BC, `mov eax,esi`. Compiler-generated: slot 0 of CViewport's primary vtable 0x01249A5C (RTTI .?AVCViewport@NUISystem@@),
//   reached from the adjustor thunks 0x0054EC50 / 0x0054EC60 (landed; they bind it as its ??_E alias).
// Naming (NF156, 2026-11-20, q4683): was the old len30-family stand-in `struct CViewport { void Dtor(); void* ScalarDeletingDestructor(); }`
//   (global class, ?ScalarDeletingDestructor@CViewport@@QAEPAXI@Z binding 0x0054EAC0 as ?Dtor@CViewport@@QAEXXZ). The destructor it calls
//   is ??1CViewport@NUISystem@@UAE@XZ (Ego_r.pdb S_PUB32 + FableWin.pdb; landed 0x0053E950 binds it).
// TU: the vtables, the thunks and this ??_G are emitted where the vtable is first needed, the constructor's TU (CViewport(long)
//   0x0054EA00); this source is that TU, as for the landed twins 0x0054EC50 / 0x0054EC60 (the constructor itself is not graded here).
// Header: engine/NUISystem_CViewport.h.
#include "engine/NUISystem_CViewport.h"
namespace NUISystem {
CViewport::CViewport(long def)
    : CChangingStateComponent(def)
    , GraphicIndex(0)
{
    GetDataFromDef();
}
}
