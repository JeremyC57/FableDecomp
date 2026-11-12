// CActiveEntityScriptBase::`scalar deleting destructor'(unsigned int flags) -- the implicit destructor inline (Entity, PEntityScript,
// then ~CSpawnedFuncBase out of line), then free this when bit 0 of flags is set; returns this
// Leaf: ??_GCActiveEntityScriptBase@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CE1200, 48 bytes, thiscall, `ret 4`.
// Identity: slot 0 of vtable 0x012C3594, the vtable the landed CActiveEntityScriptBase::CActiveEntityScriptBase 0x00CE1110 stores
//   (slots 5 / 6 the landed CActiveEntityScriptBase::OnPredicateFail 0x00CE1090 / OnInterrupted 0x00CE10A0); the members it tears
//   down are engine/CActiveEntityScriptBase.h's (Entity +0x48: ~CScriptThing 0x004AA840; PEntityScript +0x34:
//   ~CCountedPointer<CEntityScriptBase> 0x00CE1000) before the landed ~CSpawnedFuncBase 0x00CDD4C0. Ego_r.pdb / FableWin.pdb name
//   the class (GetEntity, GetFlags, GetPEntityScript); ??_G is its vtable's deleting destructor.
// Shape: `new` of the class with the constructor (retail body 0x00CE1110) defined inline is this TU's emitter of the vtable and so
//   of ??_G (descriptive emitter name, not graded).
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CActiveEntityScriptBase.h"

inline CSpawnedFuncBase::CSpawnedFuncBase(const CCharString& name, unsigned long id)
    : CSuspendableProcessMicrothread(true, 0x40, 0.1f), ID(id), LastScriptFrame(-1), CurrentScriptFrame(-1), Name(name),
      FuncPaused(false), Interrupted(false)
{
}
inline CActiveEntityScriptBase::CActiveEntityScriptBase(const CScriptThing& entity, FableNScriptUInt64 uid, unsigned long flags)
    : CSpawnedFuncBase(entity.GetName(), 1), Flags(flags), EntityUID(uid), Entity(entity)
{
}
CActiveEntityScriptBase* NewActiveEntityScriptBase(const CScriptThing& entity, FableNScriptUInt64 uid, unsigned long flags)
{
    return new CActiveEntityScriptBase(entity, uid, flags);
}
