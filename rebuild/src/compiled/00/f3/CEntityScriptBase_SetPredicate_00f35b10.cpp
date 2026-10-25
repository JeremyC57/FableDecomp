// CEntityScriptBase::SetPredicate(const NScript::CPredicateBase&) -- set the predicate on the parent script's active thread
// Leaf: ?SetPredicate@CEntityScriptBase@@QAEXABVCPredicateBase@NScript@@@Z
// Retail: Fable.exe @ 0x00F35B10, 20 bytes, `ret 4`. Catalog label "CWorldMap::AllocateMapThingUID" is propagated; the 396
// callers are entity-script Main bodies (landed CDyingVillager::Main 0x00D7F460, CHTE_AssaultGuard::Main 0x00D6AEA0, ...
// already call it SetPredicate), and Ego_r.pdb lists ?SetPredicate@CEntityScriptBase@@QAEXABVCPredicateBase@NScript@@@Z.
// Twin of IsActiveThreadTerminating 0x00F35B30 (slot 3 then the CScriptBase method).
// Callees: virtual GetParentScript() (slot 3, +0xC; pure in the CEntityScriptBase vtable 0x012C3224, derived bodies are
// `mov eax,[ecx+14h] ; ret` = ParentClass; entity-script contract of docs/engine/QUEST_SCRIPTS.md E9),
// CScriptBase::SetPredicate 0x00CB7920 (`mov ecx,[ecx+2Ch] ; jmp 0x00F35A10` = PActiveThread->SetPredicate, the
// CSpawnedFuncBase one that stores `predicate.Clone()` into PPredicate +0x2C).
// Shape: the predicate argument is pushed before the virtual call that produces the object (arguments first).
namespace NScript { class CPredicateBase; }

class CScriptBase {
public:
    void SetPredicate(const NScript::CPredicateBase& predicate);   // 0x00CB7920
};

class CEntityScriptBase {
public:
    virtual ~CEntityScriptBase();                                    // slot 0
    virtual void Main() = 0;                                         // slot 1
    virtual void Init();                                             // slot 2
    virtual CScriptBase* GetParentScript() const = 0;                // slot 3 (+0xC)
    void SetPredicate(const NScript::CPredicateBase& predicate);
private:
    void* Game;                                                      // +0x04 CGameScriptInterfaceBase*
    void* Me[3];                                                     // +0x08 CScriptThing
};

void CEntityScriptBase::SetPredicate(const NScript::CPredicateBase& predicate)
{
    GetParentScript()->SetPredicate(predicate);
}