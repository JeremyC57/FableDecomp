// Behaviour test for 0x0066A7E0 CCombatAction_GenericResponseToExplosion copy constructor.
#include <cstdio>
struct CThing { int id; };
struct CBaseIntelligentPointer {
    void* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) { }
    void* GetPItem() const { return m_pItem; }
    void SetPItem(void* item) { m_pItem = item; }
};
struct CIntelligentPointer_CThing : CBaseIntelligentPointer {
    CIntelligentPointer_CThing() { }
    CIntelligentPointer_CThing(const CIntelligentPointer_CThing& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};
static int g_baseCopies = 0;
struct CCombatActionBase {
    CCombatActionBase() { }
    CCombatActionBase(const CCombatActionBase&) { ++g_baseCopies; }
};
struct CCombatAction_GenericResponseToExplosion : CCombatActionBase {
    CIntelligentPointer_CThing PExplosion;
    float PointOfHit_X, PointOfHit_Y, PointOfHit_Z, HitDir_X, HitDir_Y, HitDir_Z;
    bool FlipFacing;
    CCombatAction_GenericResponseToExplosion() { }
    CCombatAction_GenericResponseToExplosion(const CCombatAction_GenericResponseToExplosion& o)
        : CCombatActionBase(o), PExplosion(o.PExplosion),
          PointOfHit_X(o.PointOfHit_X), PointOfHit_Y(o.PointOfHit_Y), PointOfHit_Z(o.PointOfHit_Z),
          HitDir_X(o.HitDir_X), HitDir_Y(o.HitDir_Y), HitDir_Z(o.HitDir_Z), FlipFacing(o.FlipFacing) { }
};
int main()
{
    CThing boom;
    CCombatAction_GenericResponseToExplosion a;
    a.PExplosion.SetPItem(&boom); a.PointOfHit_X = 4; a.PointOfHit_Y = 5; a.PointOfHit_Z = 6;
    a.HitDir_X = 1; a.HitDir_Y = 0; a.HitDir_Z = 0; a.FlipFacing = true;
    CCombatAction_GenericResponseToExplosion b(a);
    if (g_baseCopies != 1 || b.PExplosion.m_pItem != &boom || b.PointOfHit_Y != 5 || b.HitDir_X != 1 || !b.FlipFacing) {
        std::printf("0066a7e0_TEST FAIL\n"); return 1;
    }
    std::printf("0066a7e0_TEST PASS\n");
    return 0;
}