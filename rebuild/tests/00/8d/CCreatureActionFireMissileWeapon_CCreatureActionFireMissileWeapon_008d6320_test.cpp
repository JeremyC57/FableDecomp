// Behaviour test for 0x008D6320 CCreatureAction_FireMissileWeapon copy constructor: base copied, target re-pointed,
// shot fields copied, the shared special-shot def's reference count incremented (a null def stays null).
#include <cstdio>
#include <cstring>

struct CBaseObject { int id; };
struct CBaseIntelligentPointer {
    CBaseObject* m_pItem; int m_sets;
    CBaseIntelligentPointer() : m_pItem(0), m_sets(0) { }
    CBaseObject* GetPItem() const { return m_pItem; }
    void SetPItem(CBaseObject* p) { m_pItem = p; ++m_sets; }
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer() { }
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};
struct CCreatureActionBase {
    int m_baseValue; int m_copies;
    CCreatureActionBase() : m_baseValue(0), m_copies(0) { }
    CCreatureActionBase(const CCreatureActionBase& o) : m_baseValue(o.m_baseValue), m_copies(o.m_copies + 1) { }
};
struct CThing; struct CThingAICreature; struct CThingBuilding;
struct CMeleeCombatAbilityDef { int vt; mutable long m_refCount; };
template <class T> struct CDefPointer {
    T* m_pDef;
    CDefPointer() : m_pDef(0) { }
    CDefPointer(const CDefPointer& o) : m_pDef(o.m_pDef) { if (m_pDef) ++m_pDef->m_refCount; }
};
struct CCreatureAction_FireMissileWeapon : CCreatureActionBase {
    CIntelligentPointer<const CThing> PTarget;
    float ShotAccuracyPercentage, ShotDamageMultiplier, ShotBuildupTime; bool Decapitate; long DelayTimer;
    CDefPointer<const CMeleeCombatAbilityDef> PSpecialShotDef;
    CCreatureAction_FireMissileWeapon() { }
    CCreatureAction_FireMissileWeapon(const CCreatureAction_FireMissileWeapon& o)
        : CCreatureActionBase(o), PTarget(o.PTarget), ShotAccuracyPercentage(o.ShotAccuracyPercentage),
          ShotDamageMultiplier(o.ShotDamageMultiplier), ShotBuildupTime(o.ShotBuildupTime), Decapitate(o.Decapitate),
          DelayTimer(o.DelayTimer), PSpecialShotDef(o.PSpecialShotDef) { }
};
int main()
{
    CBaseObject tgt = { 1 }; CMeleeCombatAbilityDef def = { 0, 2 };
    CCreatureAction_FireMissileWeapon src; src.PTarget.SetPItem(&tgt); src.ShotAccuracyPercentage = 0.5f;
    src.ShotDamageMultiplier = 2.0f; src.ShotBuildupTime = 1.25f; src.Decapitate = true; src.DelayTimer = 30;
    src.PSpecialShotDef.m_pDef = &def;
    CCreatureAction_FireMissileWeapon c(src);
    CCreatureAction_FireMissileWeapon empty; empty.ShotAccuracyPercentage = 0; empty.ShotDamageMultiplier = 0;
    empty.ShotBuildupTime = 0; empty.Decapitate = false; empty.DelayTimer = 0;
    CCreatureAction_FireMissileWeapon c2(empty);
    if (c.m_copies != 1 || c.PTarget.m_pItem != &tgt || c.ShotAccuracyPercentage != 0.5f || c.ShotDamageMultiplier != 2.0f ||
        c.ShotBuildupTime != 1.25f || !c.Decapitate || c.DelayTimer != 30 || c.PSpecialShotDef.m_pDef != &def ||
        def.m_refCount != 3 || c2.PSpecialShotDef.m_pDef != 0) { std::printf("008d6320_TEST FAIL\n"); return 1; }
    std::printf("008d6320_TEST PASS\n");
    return 0;
}