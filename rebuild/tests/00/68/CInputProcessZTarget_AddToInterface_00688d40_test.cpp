// Behaviour test for CInputProcessZTarget::AddToInterface (0x00688D40): a copy of the model (counted def handle, intelligent
// pointers reduced to their item, a thing with the Dead flag and a targeting component) run through the function's logic.
#include <cstdio>

struct CDef { long IVRefCount; bool Deleted; CDef() : IVRefCount(0), Deleted(false) {} };
struct CDefPointer {
    CDef* Object;
    CDefPointer() : Object(0) {}
    CDefPointer(CDef* d) : Object(d) { if (Object) ++Object->IVRefCount; }
    CDefPointer(const CDefPointer& o) : Object(o.Object) { if (Object) ++Object->IVRefCount; }
    ~CDefPointer() { Release(); }
    void Release() { if (Object) { CDef* o = Object; if (--o->IVRefCount == 0) o->Deleted = true; Object = 0; } }
    CDefPointer& operator=(const CDefPointer& o)
    {
        if (Object != o.Object) { Release(); Object = o.Object; if (Object) ++Object->IVRefCount; }
        return *this;
    }
};
class CThing;
class CTCTargetingBase { public: CThing* Targeted; CThing* GetPTargetedThing() const { return Targeted; } };
class CThing { public:
    bool Dead; CTCTargetingBase* Targeting; CDef* HeroCombatDef;
    CThing() : Dead(false), Targeting(0), HeroCombatDef(0) {}
    bool IsDead() const { return Dead; }
    bool PeekTC(const CTCTargetingBase** out) const { if (Targeting) { *out = Targeting; return true; } return false; }
    CDefPointer GetPHeroCombatDef() const { return CDefPointer(HeroCombatDef); }
};
template<class T> class CIntelligentPointer { public:
    const T* Item;
    CIntelligentPointer() : Item(0) {}
    CIntelligentPointer(const T* p) : Item(p) {}
    const T* Get() const { return Item; }
    const T* operator->() const { return Item; }
};
template<class T> static bool IS_THING_ALIVE(const CIntelligentPointer<T>& thing) { if (thing.Get() != 0) { if (!thing->IsDead()) return true; } return false; }
static bool IS_THING_ALIVE(CThing* const& thing) { if (thing != 0) { if (!thing->IsDead()) return true; } return false; }
class CPlayer { public: CThing* Controlled; const CThing* PeekPControlledCreature() const { return Controlled; } };
class CInputProcessManager { public: int Added; };
struct CInputProcessGameBase {
    CPlayer* MyPlayer;
    bool AddToInterface(CInputProcessManager& manager) { ++manager.Added; return true; }
};
class CInputProcessZTarget : public CInputProcessGameBase { public:
    CIntelligentPointer<CThing> PCreature;
    CDefPointer PHeroCombatDef;
    long DeactivateHoldTimer;
    CIntelligentPointer<CThing> PTarget;
    bool AddToInterface(CInputProcessManager& manager)
    {
        PCreature = MyPlayer->PeekPControlledCreature();
        if (IS_THING_ALIVE(PCreature)) {
            PHeroCombatDef = PCreature->GetPHeroCombatDef();
            const CTCTargetingBase* targeting;
            if (PCreature->PeekTC(&targeting)) {
                CThing* target = targeting->GetPTargetedThing();
                if (IS_THING_ALIVE(target))
                    PTarget = target;
            }
        }
        DeactivateHoldTimer = -1;
        return CInputProcessGameBase::AddToInterface(manager);
    }
};

static int fail() { std::printf("00688d40_TEST FAIL\n"); return 1; }

int main()
{
    CInputProcessManager manager; manager.Added = 0;
    CPlayer player; player.Controlled = 0;
    CThing hero, enemy, marker;
    CDef oldDef, newDef;
    CTCTargetingBase targeting; targeting.Targeted = 0;
    {
        CInputProcessZTarget process;
        process.MyPlayer = &player;
        process.DeactivateHoldTimer = 77;
        process.PCreature = &marker;
        process.PTarget = &marker;
        process.PHeroCombatDef = CDefPointer(&oldDef);
        if (oldDef.IVRefCount != 1) return fail();

        // 1: no controlled creature -- PCreature cleared, nothing else touched, timer reset, base chained
        if (!process.AddToInterface(manager) || manager.Added != 1) return fail();
        if (process.PCreature.Get() != 0 || process.PTarget.Get() != &marker) return fail();
        if (process.PHeroCombatDef.Object != &oldDef || process.DeactivateHoldTimer != -1) return fail();

        // 2: a dead creature is latched but its def and target are not read
        player.Controlled = &hero; hero.Dead = true; hero.HeroCombatDef = &newDef; hero.Targeting = &targeting; targeting.Targeted = &enemy;
        process.DeactivateHoldTimer = 5;
        if (!process.AddToInterface(manager) || manager.Added != 2) return fail();
        if (process.PCreature.Get() != &hero || process.PTarget.Get() != &marker) return fail();
        if (process.PHeroCombatDef.Object != &oldDef || newDef.IVRefCount != 0 || process.DeactivateHoldTimer != -1) return fail();

        // 3: alive, no targeting component -- the def handle moves (old one released, temporary dropped), target untouched
        hero.Dead = false; hero.Targeting = 0;
        if (!process.AddToInterface(manager)) return fail();
        if (process.PHeroCombatDef.Object != &newDef || newDef.IVRefCount != 1) return fail();
        if (oldDef.IVRefCount != 0 || !oldDef.Deleted || newDef.Deleted) return fail();
        if (process.PTarget.Get() != &marker) return fail();

        // 4: same def again -- the count is unchanged (self-assignment guard + temporary)
        if (!process.AddToInterface(manager) || newDef.IVRefCount != 1 || newDef.Deleted) return fail();

        // 5: targeting component with no target, then with a dead target -- PTarget untouched
        hero.Targeting = &targeting; targeting.Targeted = 0;
        if (!process.AddToInterface(manager) || process.PTarget.Get() != &marker) return fail();
        targeting.Targeted = &enemy; enemy.Dead = true;
        if (!process.AddToInterface(manager) || process.PTarget.Get() != &marker) return fail();

        // 6: live target -- latched
        enemy.Dead = false; process.DeactivateHoldTimer = 9;
        if (!process.AddToInterface(manager) || process.PTarget.Get() != &enemy) return fail();
        if (process.PCreature.Get() != &hero || process.DeactivateHoldTimer != -1 || manager.Added != 7) return fail();
        if (newDef.IVRefCount != 1) return fail();
    }
    if (newDef.IVRefCount != 0 || !newDef.Deleted) return fail();
    std::printf("00688d40_TEST PASS\n");
    return 0;
}