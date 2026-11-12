// NScript::CDanyalTestScriptsData::`scalar deleting destructor'(unsigned int flags) -- the implicit destructor (~CBaseClassNonCopyable through the
// implicit ~CScriptDataBase), then free this when bit 0 of flags is set; returns this
// Leaf: ??_GCDanyalTestScriptsData@NScript@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CD4BB0, 30 bytes, thiscall, `ret 4`.
// Identity: slot 0 of vtable 0x012C273C, the vtable the class's allocator 0x00CD4B80 stores; the landed NScript::CDanyalTestScriptsData::Alloc 0x00CD4B80 (PDB name) news this class with that vtable.
//   FableWin.pdb has ??0 / OnPersist / Init / ??1 for NScript::CDanyalTestScriptsData (Ego_r sizeof 4). The body is the scalar form (only
//   bit 0 tested), so retail's slot is ??_G.
// Shape: implicit destructors store no vptr; `new` of the class (implicit inline constructor) is this TU's emitter of the vtable
//   and so of ??_G (descriptive emitter name, not graded).
#include "engine/CDanyalTestScriptsData.h"

CScriptDataBase* NewCDanyalTestScriptsData()
{
    return new NScript::CDanyalTestScriptsData;
}
