// Behaviour test for 0x008678B0 CCombatAction_GenericStrikeResponseBase(owner, thingThatHit, pointOfHit, directionOfHit).
#include <cstdio>
struct CThingCreatureBase { int id; };
struct CThing { int id; };
struct C3DVector { float X, Y, Z; };
struct CSoundPair { long FirstSound, LastSound; CSoundPair() : FirstSound(0), LastSound(0) { } };
struct CBaseIntelligentPointer {
    const void* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) { }
    void SetPItem(const void* item) { m_pItem = item; }
};
struct CIntelligentPointer_CThingConst : CBaseIntelligentPointer {
    CIntelligentPointer_CThingConst(const CThing* item) { SetPItem(item); }
};
static long g_code = 0;
struct CCombatActionBase {
    CCombatActionBase(CThingCreatureBase*, long actionCode, const CSoundPair&) { g_code = actionCode; }
};
struct CCombatAction_GenericStrikeResponseBase : CCombatActionBase {
    CIntelligentPointer_CThingConst PThingThatHit;
    float PointOfHit_X, PointOfHit_Y, PointOfHit_Z, DirectionOfHit_X, DirectionOfHit_Y, DirectionOfHit_Z;
    bool MoveOnHitOverride, CreateDustEffect;
    CCombatAction_GenericStrikeResponseBase(CThingCreatureBase* owner, const CThing* t, const C3DVector& p, const C3DVector& d)
        : CCombatActionBase(owner, 250, CSoundPair()), PThingThatHit(t),
          PointOfHit_X(p.X), PointOfHit_Y(p.Y), PointOfHit_Z(p.Z),
          DirectionOfHit_X(d.X), DirectionOfHit_Y(d.Y), DirectionOfHit_Z(d.Z),
          MoveOnHitOverride(true), CreateDustEffect(true) { }
};
int main()
{
    CThingCreatureBase owner; CThing hitter; C3DVector p = { 1, 2, 3 }; C3DVector d = { 4, 5, 6 };
    CCombatAction_GenericStrikeResponseBase a(&owner, &hitter, p, d);
    if (g_code != 250 || a.PThingThatHit.m_pItem != &hitter || a.PointOfHit_X != 1 || a.DirectionOfHit_Z != 6 ||
        !a.MoveOnHitOverride || !a.CreateDustEffect) { std::printf("008678b0_TEST FAIL\n"); return 1; }
    std::printf("008678b0_TEST PASS\n");
    return 0;
}