// CSpawnedFuncBase::`scalar deleting destructor'(unsigned int flags) -- the implicit ~CSpawnedFuncBase() out of line, then free this
// when bit 0 of flags is set; returns this
// Leaf: ??_GCSpawnedFuncBase@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CDD4A0, 30 bytes, thiscall, `ret 4`.
// Identity: slot 0 of vtable 0x012C2F9C, the vtable the landed CSpawnedFuncBase::CSpawnedFuncBase 0x00CDD450 stores (slots 1..4 the
//   CSuspendableProcessMicrothread / CASuspendableProcess virtuals 0x00A44880 / 0x00A44840 / 0x00A447D0 / 0x00A44600, slots 5 / 6
//   0x00CDD410 / 0x00CDD420); it calls the landed CSpawnedFuncBase::~CSpawnedFuncBase 0x00CDD4C0 (PPredicate, both strings, then
//   ~CSuspendableProcessMicrothread; no vptr store: the implicit destructor). Ego_r.pdb ??1CSpawnedFuncBase@@UAE@XZ.
// Shape: `new` of the class with the constructor defined inline (retail body 0x00CDD450) is this TU's emitter of the vtable and so
//   of ??_G (descriptive emitter name, not graded).
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CSpawnedFuncBase.h"

inline CSpawnedFuncBase::CSpawnedFuncBase(const CCharString& name, unsigned long id)
    : CSuspendableProcessMicrothread(true, 0x40, 0.1f), ID(id), LastScriptFrame(-1), CurrentScriptFrame(-1), Name(name),
      FuncPaused(false), Interrupted(false)
{
}
CSpawnedFuncBase* NewSpawnedFuncBase(const CCharString& name, unsigned long id)
{
    return new CSpawnedFuncBase(name, id);
}
