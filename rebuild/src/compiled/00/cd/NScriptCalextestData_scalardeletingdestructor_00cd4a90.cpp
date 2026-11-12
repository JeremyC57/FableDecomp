// NScript::CalextestData::`scalar deleting destructor'(unsigned int flags) -- the implicit destructor (~CBaseClassNonCopyable through the
// implicit ~CScriptDataBase), then free this when bit 0 of flags is set; returns this
// Leaf: ??_GCalextestData@NScript@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CD4A90, 30 bytes, thiscall, `ret 4`.
// Identity: slot 0 of vtable 0x012C2718, the vtable the class's allocator 0x00CD4A60 stores; the script-info registration at 0x00CDA28C pairs the literal "alextest" (and "PersonalScript_alextest") with that allocator; Ego_r.pdb orders ?Alloc@CalextestData just before ?Alloc@CEmptyData, as retail 0x00CD4A60 precedes 0x00CD4AC0.
//   FableWin.pdb has ??0 / OnPersist / Init / ??1 for NScript::CalextestData (Ego_r sizeof 4). The body is the scalar form (only
//   bit 0 tested), so retail's slot is ??_G.
// Shape: implicit destructors store no vptr; `new` of the class (implicit inline constructor) is this TU's emitter of the vtable
//   and so of ??_G (descriptive emitter name, not graded).
#include "engine/CalextestData.h"

CScriptDataBase* NewCalextestData()
{
    return new NScript::CalextestData;
}
