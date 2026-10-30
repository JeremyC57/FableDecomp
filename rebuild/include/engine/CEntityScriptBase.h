#ifndef FABLE_ENGINE_CENTITYSCRIPTBASE_H
#define FABLE_ENGINE_CENTITYSCRIPTBASE_H
// Retail model (hand-written 2026-10-29, wave 1 class-model migration). No RTTI; vtable 0x012C3224 (stored by
// ~CEntityScriptBase 0x00F35B40), 7 slots: 0 scalar deleting dtor 0x00CDEBF0, 1 Main (_purecall), 2 Init 0x00CDEBB0,
// 3 GetParentScript (_purecall), 4 OnPersist 0x00CDEBC0 (`ret 4`), 5 OnPredicateFail 0x00CDEBD0, 6 OnInterrupted 0x00CDEBE0.
// Names: Ego_r.pdb / FableWin.pdb ?Init/?OnPersist/?OnPredicateFail/?OnInterrupted@CEntityScriptBase@@UAE..; the pure slots
// are named from every entity class's overrides (?Main@<entity>@..@@EAEXXZ, ?GetParentScript@<entity>@..@@UBEPAVCScriptBase@@XZ;
// e.g. CAT_BanditAlly vtable 0x012F7D88 slot 3 0x00F03840 `mov eax,[ecx+14h]` returns ParentClass). Slot order 4/5/6 follows
// the retail bodies (`ret 4` is the CPersistContext& slot) and the landed entity rows (OnPredicateFail = slot 5, chunk 579
// 0x00F1A8F0); 5 vs 6 between OnPredicateFail / OnInterrupted rests on those rows plus CSpawnedFuncBase's same pair order.
// Main's access in the base is unproven (every override is private, EAE): declared public here.
// Layout (Ego_r.pdb, 0x14, also the retail Alloc sizes): Game +0x04, Me +0x08. SetPredicate 0x00F35B10 (calls slot 3 then
// CScriptBase::SetPredicate 0x00CB7920), IsActiveThreadTerminating 0x00F35B30 (const: Ego_r ..QBE_NXZ).
// Contract: CScriptThing comes from engine/CScriptThing.h, so the includer defines CCountedPointer<T> first.
// The ctor is inline (wave 7): every entity-script Alloc expands it -- vtable 0x012C3224 store, Game +0x04 (the parent's
// Game, `mov eax,[ebx+40h]`), then the out-of-line CScriptThing copy 0x004ABE90 into Me +0x08 (e.g. 0x00F1B800). Ego_r.pdb
// (release) has no ??0CEntityScriptBase; FableWin.pdb (debug) lists it out of line (0x035E6BC0).
#ifndef FABLE_RETAIL_NSCRIPT
#error engine/CEntityScriptBase.h is the retail NScript model: define FABLE_RETAIL_NSCRIPT before including it
#else
#include "rebuild_abi.h"
#include "engine/CScriptThing.h"
class CGameScriptInterfaceBase;

class CEntityScriptBase {
public:
    CEntityScriptBase(CGameScriptInterfaceBase& game, const CScriptThing& me) : Game(&game), Me(me) {}
    virtual ~CEntityScriptBase();                                        // slot 0  0x00CDEBF0 (scalar deleting); 0x00F35B40
    virtual void Main() = 0;                                             // slot 1
    virtual void Init();                                                 // slot 2  0x00CDEBB0
    virtual CScriptBase* GetParentScript() const = 0;                    // slot 3
    virtual void OnPersist(CPersistContext&);                            // slot 4  0x00CDEBC0
    virtual void OnPredicateFail();                                      // slot 5  0x00CDEBD0
    virtual void OnInterrupted();                                        // slot 6  0x00CDEBE0
    void SetPredicate(const NScript::CPredicateBase&);                   // 0x00F35B10
    bool IsActiveThreadTerminating() const;                              // 0x00F35B30
    CGameScriptInterfaceBase* Game;                                      // +0x04
    CScriptThing              Me;                                        // +0x08
};
FABLE_STATIC_ASSERT(sizeof(CEntityScriptBase) == 0x14);

#endif // FABLE_RETAIL_NSCRIPT
#endif // FABLE_ENGINE_CENTITYSCRIPTBASE_H
