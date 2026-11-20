#pragma optimize("s", on)
// NUISystem::CActionParamCCountedPointer<NUISystem::CComponent>::CActionParamCCountedPointer(CCountedPointer<CComponent>& value) -- type tag 9
//   (UI_ACTION_PARAM_TYPE_COUNTED_POINTER) at +4, own vtable, the counted pointer copy-constructed at +8
// Leaf: ??0?$CActionParamCCountedPointer@VCComponent@NUISystem@@@NUISystem@@QAE@AAV?$CCountedPointer@VCComponent@NUISystem@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004854DF, length 34 bytes.
// Identity: Ego_r.pdb / FableWin.pdb ??0?$CActionParamCCountedPointer@VCComponent@NUISystem@@@NUISystem@@QAE@AAV?$CCountedPointer@VCComponent@
//   NUISystem@@@@@Z (engine/NUISystem_CActionParamCCountedPointer.h); every landed referencer binds it (0x005C3E91 in reloc_bindings.tsv).
//   Vtable 0x01236794 = CActionParamCCountedPointer<CComponent>.
// Callees: CCountedPointer<NUISystem::CComponent>::CCountedPointer(const CCountedPointer&) @ 0x00428928 (??0?$CCountedPointer@VCComponent@
//   NUISystem@@@@QAE@ABV0@@Z, the entry q6142 spelling).
// Pragma: #pragma optimize("s", on) -- the argument is pushed straight from its stack slot (`push dword ptr [esp+8]`), the
//   size-optimised pass-through shape. The vtable store sinks to just before the member constructor call.
// Naming (NF156, 2026-11-20, q6142): moved off the row-local CCountedPointer / CActionParam / CActionParamCCountedPointer models (the
//   global ??0CActionParamCCountedPointer@@QAE@ABUCCountedPointer@@@Z, binding 0x00428928 as ??0CCountedPointer@@QAE@ABU0@@Z) onto the
//   shared header; the definition is the template member, explicitly instantiated for CComponent.
#define FABLE_RETAIL_CACTIONPARAMCCOUNTEDPOINTER
#include "engine/NUISystem_CActionParamCCountedPointer.h"
namespace NUISystem {
template <class T>
CActionParamCCountedPointer<T>::CActionParamCCountedPointer(CCountedPointer<T>& value)
    : CActionParam(UI_ACTION_PARAM_TYPE_COUNTED_POINTER)
    , Value(value)
{
}
template CActionParamCCountedPointer<CComponent>::CActionParamCCountedPointer(CCountedPointer<CComponent>& value);
}
