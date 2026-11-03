#ifndef FABLE_ENGINE_CPREDICATEBASE_H
#define FABLE_ENGINE_CPREDICATEBASE_H
// Retail model (hand-written 2026-10-29, wave 1 class-model migration): NScript::CPredicateBase, the abstract script
// predicate (Ego_r.pdb 4 bytes, no members). No RTTI; vtable 0x012C2FF4: 0 _purecall, 1 _purecall, 2 scalar deleting dtor
// 0x00CDDB40 (the dtor is inline: that body is the vtable store + conditional delete). The pure slots are named from every
// predicate's overrides: slot 0 `bool operator()() const` (Ego_r + FableWin ??RCIsThingAlive@NScript@@UBE_NXZ; CIsThingAlive
// slot 0 0x00CDF120 tail-jumps to CScriptThing::IsAlive), slot 1 `CPredicateBase* Clone() const` (?Clone@..@@UBEPAVCPredicateBase@2@XZ;
// CIsThingAlive slot 1 0x00CDF130 news 0x10 bytes). Declaration order = vtable order (dtor last).
// Ctor / copy ctor / dtor are inline: retail stores this vtable inline at every derived construction / destruction
// (0x00E24760, 0x00CDF160) and has no out-of-line instance (FableWin.pdb has ??0 / ??1 only as debug-build copies).
// FableWin.pdb also has the static ?InitialiseGameInterface@CPredicateBase@NScript@@SIXABVCGameScriptInterfaceBase@@@Z.
#ifndef FABLE_RETAIL_NSCRIPT
#error engine/CPredicateBase.h is the retail NScript model: define FABLE_RETAIL_NSCRIPT before including it
#else
#include "rebuild_abi.h"
class CGameScriptInterfaceBase;

namespace NScript {
class CPredicateBase {
public:
    CPredicateBase() { }
    virtual bool operator()() const = 0;                                 // slot 0
    virtual CPredicateBase* Clone() const = 0;                           // slot 1
    virtual ~CPredicateBase() { }                                        // slot 2  0x00CDDB40 (scalar deleting)
    static void FABLE_FASTCALL InitialiseGameInterface(const CGameScriptInterfaceBase&);
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CPredicateBase) == 0x4);

#endif // FABLE_RETAIL_NSCRIPT
#endif // FABLE_ENGINE_CPREDICATEBASE_H
