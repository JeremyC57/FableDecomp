// CInputEvent::`scalar deleting destructor'(unsigned int flags) -- the empty destructor (??1CInputEvent@@QAE@XZ 0x00A03B30), then operator delete
//   on flag bit 0; returns this
// Leaf: ??_GCInputEvent@@QAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009F61C0, 30 bytes, thiscall `ret 4`.
// Names (NF142, 2026-11-20, q4299 binder): FableWin.pdb ??_GCInputEvent@@QAEPAXI@Z (0x030D4AE0, beside ??_GCInputEventPackage 0x030D4B60;
//   retail 0x009F61C0 sits in the CInputEventPackage rows 0x009F50A0..0x009F6E00), and the body is byte for byte the scalar deleting
//   destructor VC7.1 emits for engine/CInputEvent.h (call ~CInputEvent, `test byte [esp+8],1`, operator delete, `mov eax,esi`). Was the
//   stand-in ?CLandscapeBackgroundPatch_vector_deleting_destructor@@YIPAXPAVCLandscapeBackgroundPatch@@HI@Z binding 0x00A03B30 as
//   ?CLandscapeBackgroundPatch_dtor_00a03b30@@YIXPAVCLandscapeBackgroundPatch@@@Z. No landed row references 0x009F61C0 (exe scan).
// Shape: a compiler-generated COMDAT (CInputEvent is not polymorphic: VC7.1 emits ??_G for a delete of the class); the TU-internal
//   scaffolding function below makes this TU emit it as a pick-any COMDAT (the nf86 pattern).
#include "fable_compiler.h"
#include "engine/CInputEvent.h"
namespace {
// Fidelity: scaffolding, not retail: the smallest delete of a CInputEvent, so this TU emits CInputEvent's scalar deleting destructor (a
// pick-any COMDAT). Internal to this TU.
void ReferenceInputEventScalarDeletingDestructor(CInputEvent* event)
{
    delete event;
}
}
