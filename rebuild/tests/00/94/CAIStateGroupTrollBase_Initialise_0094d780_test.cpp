// Behaviour test for CAIStateGroup_TrollBase::Initialise (0x0094D780): the same logic through a small counted-def model.
#include <cstdio>

struct CDef {                                   // counted def pointee
    mutable long RefCount;
    mutable long Deleted;
    CDef() : RefCount(1), Deleted(0) {}
    virtual ~CDef() {}
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) {}
    ~CDefPointer() { Release(); }
    void Set(T* p) { Release(); Object = p; if (Object) ++Object->RefCount; }
    void Release() { if (Object) { if (--Object->RefCount == 0) Object->Deleted = 1; Object = 0; } }
    T* operator->() const { return Object; }
};
class CExplosionDef : public CDef { public: float ExplosionRadius; };
class CTrapDef : public CDef { public: long ExplosionDefIndex; };
class CAreaOfEffectAttackDef : public CDef { public: long TrapIndex; long ExplosionIndex; };
class CThingBaseDef : public CDef { public:
    CAreaOfEffectAttackDef* Aoe; CTrapDef* Trap; CExplosionDef* Explosion;
    CThingBaseDef() : Aoe(0), Trap(0), Explosion(0) {}
    bool GetSubDef(CDefPointer<const CAreaOfEffectAttackDef>* out) const { if (!Aoe) return false; ((CDefPointer<CAreaOfEffectAttackDef>*)out)->Set(Aoe); return true; }
    bool GetSubDef(CDefPointer<const CTrapDef>* out) const { if (!Trap) return false; ((CDefPointer<CTrapDef>*)out)->Set(Trap); return true; }
    bool GetSubDef(CDefPointer<const CExplosionDef>* out) const { if (!Explosion) return false; ((CDefPointer<CExplosionDef>*)out)->Set(Explosion); return true; }
};
class CGameDefinitionManager { public:
    CThingBaseDef* Defs[8];
    static CGameDefinitionManager& Get() { static CGameDefinitionManager m; return m; }
    bool GetDef(long index, CDefPointer<const CThingBaseDef>* out) const
    {
        if (index > 0 && index < 8 && Defs[index]) { ((CDefPointer<CThingBaseDef>*)out)->Set(Defs[index]); return true; }
        return false;
    }
};
class CTCPerceiveThing { public: float GiveUpChaseRadius; float GetGiveUpChaseRadius() const { return GiveUpChaseRadius; } };
class CThing { public: CTCPerceiveThing* Perceive; CThingBaseDef* PBaseDef; };
class CAIBrain { public: CThing* Creature; };

struct CAIStateGroup_TrollBase {
    CAIBrain* PBrain;
    long BaseInitialised;
    float TROLL_MAX_THROWING_DISTANCE, TROLL_MIN_THROWING_DISTANCE, TROLL_STOP_POUNDING_DISTANCE, TROLL_STOP_THROWING_DISTANCE;
    void BaseInitialise(CAIBrain& brain) { PBrain = &brain; ++BaseInitialised; }
    void Initialise(CAIBrain& brain)
    {
        BaseInitialise(brain);
        CTCPerceiveThing* perceive = PBrain->Creature->Perceive;
        TROLL_MAX_THROWING_DISTANCE = perceive->GetGiveUpChaseRadius();
        CDefPointer<const CAreaOfEffectAttackDef> aoe;
        if (PBrain->Creature->PBaseDef->GetSubDef(&aoe)) {
            long explosion_index = 0;
            if (aoe->ExplosionIndex != 0)
                explosion_index = aoe->ExplosionIndex;
            else if (aoe->TrapIndex != 0) {
                CDefPointer<const CThingBaseDef> trap_thing;
                if (CGameDefinitionManager::Get().GetDef(aoe->TrapIndex, &trap_thing)) {
                    CDefPointer<const CTrapDef> trap;
                    if (trap_thing->GetSubDef(&trap))
                        explosion_index = trap->ExplosionDefIndex;
                }
            }
            if (explosion_index != 0) {
                CDefPointer<const CThingBaseDef> explosion_thing;
                if (CGameDefinitionManager::Get().GetDef(explosion_index, &explosion_thing)) {
                    CDefPointer<const CExplosionDef> explosion;
                    if (explosion_thing->GetSubDef(&explosion)) {
                        TROLL_MIN_THROWING_DISTANCE = explosion->ExplosionRadius;
                        TROLL_STOP_POUNDING_DISTANCE = explosion->ExplosionRadius;
                        TROLL_STOP_THROWING_DISTANCE = explosion->ExplosionRadius + 1.0f;
                    }
                }
            } else {
                TROLL_MAX_THROWING_DISTANCE = 28.0f;
                TROLL_MIN_THROWING_DISTANCE = 8.0f;
                TROLL_STOP_POUNDING_DISTANCE = 11.0f;
                TROLL_STOP_THROWING_DISTANCE = 8.0f;
            }
        }
    }
};

static CThingBaseDef g_creatureDef, g_trapThing, g_explosionThing, g_emptyThing;
static CAreaOfEffectAttackDef g_aoe;
static CTrapDef g_trap;
static CExplosionDef g_explosion;

static bool Run(long trapIndex, long explosionIndex, bool hasAoe, float m0, float m1, float m2, float m3, const char* what)
{
    CTCPerceiveThing perceive; perceive.GiveUpChaseRadius = 40.0f;
    CThing thing; thing.Perceive = &perceive; thing.PBaseDef = &g_creatureDef;
    CAIBrain brain; brain.Creature = &thing;
    g_creatureDef.Aoe = hasAoe ? &g_aoe : 0;
    g_aoe.TrapIndex = trapIndex; g_aoe.ExplosionIndex = explosionIndex;
    CAIStateGroup_TrollBase group;
    group.PBrain = 0; group.BaseInitialised = 0;
    group.TROLL_MAX_THROWING_DISTANCE = -1.0f; group.TROLL_MIN_THROWING_DISTANCE = -2.0f;
    group.TROLL_STOP_POUNDING_DISTANCE = -3.0f; group.TROLL_STOP_THROWING_DISTANCE = -4.0f;
    group.Initialise(brain);
    bool ok = group.PBrain == &brain && group.BaseInitialised == 1
        && group.TROLL_MAX_THROWING_DISTANCE == m0 && group.TROLL_MIN_THROWING_DISTANCE == m1
        && group.TROLL_STOP_POUNDING_DISTANCE == m2 && group.TROLL_STOP_THROWING_DISTANCE == m3
        && g_aoe.RefCount == 1 && g_trap.RefCount == 1 && g_explosion.RefCount == 1 && g_trapThing.RefCount == 1
        && g_explosionThing.RefCount == 1 && g_emptyThing.RefCount == 1 && g_creatureDef.RefCount == 1;
    if (!ok)
        std::printf("case failed: %s (%g %g %g %g)\n", what, group.TROLL_MAX_THROWING_DISTANCE, group.TROLL_MIN_THROWING_DISTANCE,
                    group.TROLL_STOP_POUNDING_DISTANCE, group.TROLL_STOP_THROWING_DISTANCE);
    return ok;
}

int main()
{
    CGameDefinitionManager& manager = CGameDefinitionManager::Get();
    for (int i = 0; i < 8; ++i) manager.Defs[i] = 0;
    manager.Defs[2] = &g_trapThing;      g_trapThing.Trap = &g_trap;
    manager.Defs[3] = &g_explosionThing; g_explosionThing.Explosion = &g_explosion;
    manager.Defs[4] = &g_emptyThing;     // a thing def with no trap / explosion sub-def
    g_explosion.ExplosionRadius = 6.5f;
    bool ok = true;
    // no area-of-effect sub-def: only the maximum is set
    ok = Run(2, 3, false, 40.0f, -2.0f, -3.0f, -4.0f, "no aoe") && ok;
    // direct explosion index
    ok = Run(0, 3, true, 40.0f, 6.5f, 6.5f, 7.5f, "direct explosion") && ok;
    // the explosion index wins over the trap
    g_trap.ExplosionDefIndex = 4;
    ok = Run(2, 3, true, 40.0f, 6.5f, 6.5f, 7.5f, "explosion before trap") && ok;
    // through the trap def
    g_trap.ExplosionDefIndex = 3;
    ok = Run(2, 0, true, 40.0f, 6.5f, 6.5f, 7.5f, "via trap") && ok;
    // neither index: the defaults (the maximum too)
    ok = Run(0, 0, true, 28.0f, 8.0f, 11.0f, 8.0f, "defaults") && ok;
    // a trap without an explosion: defaults
    g_trap.ExplosionDefIndex = 0;
    ok = Run(2, 0, true, 28.0f, 8.0f, 11.0f, 8.0f, "trap without explosion") && ok;
    // a trap thing def that is missing / has no CTrapDef: defaults
    ok = Run(7, 0, true, 28.0f, 8.0f, 11.0f, 8.0f, "missing trap def") && ok;
    ok = Run(4, 0, true, 28.0f, 8.0f, 11.0f, 8.0f, "trap thing without trap sub-def") && ok;
    // an explosion index whose def is missing or has no CExplosionDef: nothing but the maximum changes
    ok = Run(0, 7, true, 40.0f, -2.0f, -3.0f, -4.0f, "missing explosion def") && ok;
    ok = Run(0, 4, true, 40.0f, -2.0f, -3.0f, -4.0f, "explosion thing without sub-def") && ok;
    if (!ok) { std::printf("0094d780_TEST FAIL\n"); return 1; }
    std::printf("0094d780_TEST PASS\n");
    return 0;
}