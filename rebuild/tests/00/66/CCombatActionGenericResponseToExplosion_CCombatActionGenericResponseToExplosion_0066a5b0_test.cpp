// Behaviour test for 0x0066A5B0 CCombatAction_GenericResponseToExplosion(owner, explosion, pointOfHit, hitDir).
#include <cstdio>
struct CThingCreatureBase { int id; };
struct CThing { int id; };
struct C3DVector { float X, Y, Z; };
struct CSoundPair { long FirstSound, LastSound; CSoundPair() : FirstSound(0), LastSound(0) { } };
struct CBaseIntelligentPointer {
    void* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) { }
    void SetPItem(void* item) { m_pItem = item; }
};
struct CIntelligentPointer_CThing : CBaseIntelligentPointer {
    CIntelligentPointer_CThing(CThing* item) { SetPItem(item); }
};
static CThingCreatureBase* g_owner = 0; static long g_code = 0; static long g_sound = -1;
struct CCombatActionBase {
    CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound)
    { g_owner = owner; g_code = actionCode; g_sound = sound.FirstSound + sound.LastSound; }
};
struct CCombatAction_GenericResponseToExplosion : CCombatActionBase {
    CIntelligentPointer_CThing PExplosion;
    float PointOfHit_X, PointOfHit_Y, PointOfHit_Z, HitDir_X, HitDir_Y, HitDir_Z;
    bool FlipFacing;
    CCombatAction_GenericResponseToExplosion(CThingCreatureBase* owner, CThing* explosion, const C3DVector& p, const C3DVector& d)
        : CCombatActionBase(owner, 250, CSoundPair()), PExplosion(explosion),
          PointOfHit_X(p.X), PointOfHit_Y(p.Y), PointOfHit_Z(p.Z), HitDir_X(d.X), HitDir_Y(d.Y), HitDir_Z(d.Z),
          FlipFacing(false) { }
};
int main()
{
    CThingCreatureBase owner; CThing boom; C3DVector p = { 1, 2, 3 }; C3DVector d = { 0, 0, -1 };
    CCombatAction_GenericResponseToExplosion a(&owner, &boom, p, d);
    if (g_owner != &owner || g_code != 250 || g_sound != 0 || a.PExplosion.m_pItem != &boom || a.PointOfHit_Z != 3 ||
        a.HitDir_Z != -1 || a.FlipFacing) { std::printf("0066a5b0_TEST FAIL\n"); return 1; }
    std::printf("0066a5b0_TEST PASS\n");
    return 0;
}