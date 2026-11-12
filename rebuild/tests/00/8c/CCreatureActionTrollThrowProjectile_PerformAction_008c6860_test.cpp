// Behaviour test for 0x008C6860 CCreatureAction_TrollThrowProjectile::PerformAction (standalone copy of the model).
// Checks: nothing happens without a carrying component or with a missing / dead shot thing; otherwise the shot leaves the carrying
// component, gets the shot component named by the shot def, and that component is initialised with the creature as shooter, no
// weapon, the creature's accuracy, 1.0 / 0.0 and a null ability, at TargetPos or at the target thing by UseTargetPos; every def
// reference taken is given back (the by-value shot def copy is owned by Init).
#include <cstdio>
#include <cstring>

struct CDefPointee { long RefCount; long Deleted; CDefPointee() : RefCount(1), Deleted(0) {} };
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) {}
    CDefPointer(const CDefPointer& o) : Object(o.Object) { if (Object) ++Object->RefCount; }
    ~CDefPointer() { Release(); }
    void Set(T* p) { Release(); Object = p; if (Object) ++Object->RefCount; }
    void Release() { if (Object) { if (--Object->RefCount == 0) Object->Deleted = 1; Object = 0; } }
    T* operator->() const { return Object; }
};
class CThingShotDef : public CDefPointee { public: const char* ShotDefinitionClassName; };
class CWeaponDef : public CDefPointee { public: long Shot; };
class CMeleeCombatAbilityDef : public CDefPointee { public:};
class C3DVector { public: float X, Y, Z; };

class CThing;
struct CShotTC {
    int initKind;                 // 1 = position overload, 2 = thing overload
    CThingShotDef* def; CThing* shooter; CThing* weapon; const C3DVector* pos; const CThing* target;
    float accuracy, damageMultiplier, buildupTime; bool abilityNull;
    CShotTC() : initKind(0), def(0), shooter(0), weapon(0), pos(0), target(0), accuracy(0), damageMultiplier(0), buildupTime(0), abilityNull(false) {}
    void Init(CDefPointer<CThingShotDef> d, CThing* s, CThing* w, const C3DVector& p, float a, float m, float b,
              const CDefPointer<CMeleeCombatAbilityDef>& ability)
    { initKind = 1; def = d.Object; shooter = s; weapon = w; pos = &p; accuracy = a; damageMultiplier = m; buildupTime = b; abilityNull = ability.Object == 0; }
    void Init(CDefPointer<CThingShotDef> d, CThing* s, CThing* w, const CThing* t, float a, float m, float b,
              const CDefPointer<CMeleeCombatAbilityDef>& ability)
    { initKind = 2; def = d.Object; shooter = s; weapon = w; target = t; accuracy = a; damageMultiplier = m; buildupTime = b; abilityNull = ability.Object == 0; }
};
class CTCCarrying { public:
    CThing* removed; int removeCount;
    CTCCarrying() : removed(0), removeCount(0) {}
    void RemoveThing(CThing& thing) { removed = &thing; ++removeCount; }
};
class CThing { public:
    CTCCarrying* carrying;        // the TCI_CARRYING component, 0 = HasTC false
    bool dead;
    CWeaponDef* weaponSubDef;     // what the base def's GetSubDef<CWeaponDef> yields
    long shotAccuracy;
    CShotTC shotTC; const char* addedName; int addCount;
    CThing() : carrying(0), dead(false), weaponSubDef(0), shotAccuracy(0), addedName(0), addCount(0) {}
    bool GetTC(CTCCarrying** out) { if (carrying) { *out = carrying; return true; } return false; }
    bool IsDead() const { return dead; }
    bool GetSubDef(CDefPointer<CWeaponDef>* out) { out->Set(weaponSubDef); return true; }
    long GetShotAccuracyPercentage() const { return shotAccuracy; }
    CShotTC* AddTC(const char* name, bool activate, const void* params)
    { if (!activate || params) return 0; addedName = name; ++addCount; return &shotTC; }
};
class CDefinitionManager { public:
    CThingShotDef* defs[8];
    bool GetDef(long index, CDefPointer<CThingShotDef>* out) { if (index > 0 && defs[index]) { out->Set(defs[index]); return true; } return false; }
};
static bool IS_THING_ALIVE(CThing* thing) { if (thing != 0) { if (!thing->IsDead()) return true; } return false; }

class CCreatureAction_TrollThrowProjectile { public:
    CThing* PCreature; CThing* PTarget; CThing* PShot; C3DVector TargetPos; bool UseTargetPos;
    CDefinitionManager* DefinitionManager;
    void PerformAction()
    {
        CTCCarrying* carrying;
        if (PCreature->GetTC(&carrying)) {
            if (IS_THING_ALIVE(PShot)) {
                carrying->RemoveThing(*PShot);
                CDefPointer<CWeaponDef> weaponDef;
                PCreature->GetSubDef(&weaponDef);
                CDefPointer<CThingShotDef> shotDef;
                DefinitionManager->GetDef(weaponDef->Shot, &shotDef);
                long accuracy = PCreature->GetShotAccuracyPercentage();
                CShotTC* shot;
                {
                    const char* name = shotDef->ShotDefinitionClassName;
                    shot = PShot->AddTC(name, true, 0);
                }
                if (UseTargetPos) {
                    CDefPointer<CMeleeCombatAbilityDef> ability;
                    shot->Init(shotDef, PCreature, 0, TargetPos, (float)accuracy, 1.0f, 0.0f, ability);
                }
                else {
                    CDefPointer<CMeleeCombatAbilityDef> ability;
                    CThing* shooter = PCreature;
                    shot->Init(shotDef, shooter, 0, PTarget, (float)accuracy, 1.0f, 0.0f, ability);
                }
            }
        }
    }
};

static int Fail(int step) { std::printf("step %d\n", step); std::printf("008c6860_TEST FAIL\n"); return 1; }

int main()
{
    CThingShotDef shotDef; shotDef.ShotDefinitionClassName = "CTCShotDefinitionThrow";
    CWeaponDef weaponDef; weaponDef.Shot = 3;
    CDefinitionManager manager; std::memset(manager.defs, 0, sizeof(manager.defs)); manager.defs[3] = &shotDef;
    CTCCarrying carrying;
    CThing creature, target, shotThing;
    creature.weaponSubDef = &weaponDef; creature.shotAccuracy = 73;
    CCreatureAction_TrollThrowProjectile action;
    action.PCreature = &creature; action.PTarget = &target; action.PShot = &shotThing; action.DefinitionManager = &manager;
    action.TargetPos.X = 1.0f; action.TargetPos.Y = 2.0f; action.TargetPos.Z = 3.0f; action.UseTargetPos = true;

    // 1. no carrying component: nothing happens
    action.PerformAction();
    if (shotThing.addCount != 0 || carrying.removeCount != 0) return Fail(1);
    // 2. carrying, but no shot thing / a dead one: nothing happens
    creature.carrying = &carrying;
    action.PShot = 0; action.PerformAction();
    if (carrying.removeCount != 0) return Fail(2);
    action.PShot = &shotThing; shotThing.dead = true; action.PerformAction();
    if (carrying.removeCount != 0 || shotThing.addCount != 0) return Fail(3);
    // 3. alive shot, position target
    shotThing.dead = false; action.PerformAction();
    if (carrying.removeCount != 1 || carrying.removed != &shotThing) return Fail(4);
    if (shotThing.addCount != 1 || std::strcmp(shotThing.addedName, "CTCShotDefinitionThrow") != 0) return Fail(5);
    CShotTC& tc = shotThing.shotTC;
    if (tc.initKind != 1 || tc.def != &shotDef || tc.shooter != &creature || tc.weapon != 0 || tc.pos != &action.TargetPos) return Fail(6);
    if (tc.accuracy != 73.0f || tc.damageMultiplier != 1.0f || tc.buildupTime != 0.0f || !tc.abilityNull) return Fail(7);
    if (shotDef.RefCount != 1 || weaponDef.RefCount != 1 || shotDef.Deleted || weaponDef.Deleted) return Fail(8);
    // 4. alive shot, thing target
    action.UseTargetPos = false; creature.shotAccuracy = 40; tc = CShotTC(); action.PerformAction();
    if (carrying.removeCount != 2 || shotThing.addCount != 2) return Fail(9);
    if (tc.initKind != 2 || tc.def != &shotDef || tc.shooter != &creature || tc.weapon != 0 || tc.target != &target) return Fail(10);
    if (tc.accuracy != 40.0f || tc.damageMultiplier != 1.0f || tc.buildupTime != 0.0f || !tc.abilityNull) return Fail(11);
    if (shotDef.RefCount != 1 || weaponDef.RefCount != 1 || shotDef.Deleted || weaponDef.Deleted) return Fail(12);
    std::printf("008c6860_TEST PASS\n");
    return 0;
}