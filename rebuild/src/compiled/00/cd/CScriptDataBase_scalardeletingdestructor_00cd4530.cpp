// CScriptDataBase::`scalar deleting destructor'(unsigned int flags) -- the implicit destructor (~CBaseClassNonCopyable), then free
// this when bit 0 of flags is set; returns this
// Leaf: ??_GCScriptDataBase@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CD4530, 30 bytes, thiscall, `ret 4`.
// Identity: slot 0 of vtable 0x012C270C, whose slots 1 / 2 are _purecall (the abstract OnPersist / Init of CScriptDataBase) and which
//   only 0x00CD4500 stores (CBaseClassNonCopyable() then this vtable: FableWin.pdb ??0CScriptDataBase@@QAE@XZ); every master-script
//   data class derives from it (the NScript::C*Data::Alloc PDB names return CScriptDataBase*, and their vtables 0x012C2718 ..
//   0x012C2748 follow it with the same slot 0 / OnPersist / Init shape). FableWin.pdb ??1CScriptDataBase@@UAE@XZ is the implicit
//   virtual destructor (it stores no vptr: the body calls ~CBaseClassNonCopyable 0x0099A300 directly).
// Shape: the opt-in constructor (retail 0x00CD4500), defined inline here, expands into the derived NScript::CEmptyData
//   constructor of the emitter below, which makes this TU emit the CScriptDataBase vtable and so ??_G (descriptive emitter name,
//   not graded).
#define FABLE_RETAIL_CSCRIPTDATABASE_CTOR
#include "engine/CEmptyData.h"

inline CScriptDataBase::CScriptDataBase()
{
}
CScriptDataBase* NewScriptDataEmitter()
{
    return new NScript::CEmptyData;
}
