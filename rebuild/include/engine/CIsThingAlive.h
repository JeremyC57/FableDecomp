#ifndef FABLE_ENGINE_CISTHINGALIVE_H
#define FABLE_ENGINE_CISTHINGALIVE_H
// Retail model (hand-written 2026-10-29, wave 1 class-model migration): NScript::CIsThingAlive : CPredicateBase (Ego_r.pdb
// 0x10: Thing +0x04). No RTTI; vtable 0x012C32F0: 0 operator() 0x00CDF120 (`jmp [eax+12Ch]` = Thing.IsAlive(), CScriptThing
// slot 75), 1 Clone 0x00CDF130, 2 scalar deleting dtor 0x00CDF160; ~CIsThingAlive 0x00E241B0. Names: Ego_r.pdb + FableWin.pdb
// ??RCIsThingAlive@NScript@@UBE_NXZ, ?Clone@CIsThingAlive@NScript@@UBEPAVCPredicateBase@2@XZ, ??1CIsThingAlive@NScript@@UAE@XZ;
// the ctor ??0CIsThingAlive@NScript@@QAE@ABVCScriptThing@@@Z is FableWin-only (retail expands it inline: a row that needs
// the inline body defines it out of class after the include).
// Contract: engine/CScriptThing.h needs CCountedPointer<T> defined before the include.
#ifndef FABLE_RETAIL_NSCRIPT
#error engine/CIsThingAlive.h is the retail NScript model: define FABLE_RETAIL_NSCRIPT before including it
#else
#include "rebuild_abi.h"
#include "engine/CPredicateBase.h"
#include "engine/CScriptThing.h"

namespace NScript {
class CIsThingAlive : public CPredicateBase {
public:
    CIsThingAlive(const CScriptThing&);
    virtual bool operator()() const;                                     // slot 0  0x00CDF120
    virtual CPredicateBase* Clone() const;                               // slot 1  0x00CDF130
    virtual ~CIsThingAlive();                                            // slot 2  0x00CDF160 (scalar deleting); 0x00E241B0
    CScriptThing Thing;                                                  // +0x04
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CIsThingAlive) == 0x10);

#endif // FABLE_RETAIL_NSCRIPT
#endif // FABLE_ENGINE_CISTHINGALIVE_H
