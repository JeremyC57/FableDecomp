#include <cstdio>
class C3DVector { public: float x, y, z; };
class CThing { public: C3DVector pos; long region; const C3DVector& GetPosition() { return pos; } long GetRegionAtPosition(const C3DVector& p) { return region + (long)p.x; } };
struct CBaseIntelligentPointer { virtual void v00() {} CThing* m_pItem; CThing* GetPItem() { return m_pItem; } };
class CAIBrain { public: unsigned char _pad[0x18]; CThing* Creature; };
class CAIStateGroupBase { public: virtual ~CAIStateGroupBase() {} CAIBrain* PBrain; unsigned char _pad[0xc]; };
static bool g_baseOk = true;
struct CAIStateGroup_MeleeCombatNearbyBase : CAIStateGroupBase { CBaseIntelligentPointer PEnemy; bool IsStillRunnable(bool) { return g_baseOk; } };
struct CAIStateGroup_MeleeCombatNearby2 : CAIStateGroup_MeleeCombatNearbyBase { bool Archer; bool IsStillRunnable(bool checkEnemy); };
bool CAIStateGroup_MeleeCombatNearby2::IsStillRunnable(bool checkEnemy)
{
    if (!CAIStateGroup_MeleeCombatNearbyBase::IsStillRunnable(checkEnemy)) return false;
    CThing* enemy = PEnemy.GetPItem(); CThing* creature = PBrain->Creature;
    long enemyRegion = enemy->GetRegionAtPosition(PEnemy.GetPItem()->GetPosition());
    long creatureRegion = creature->GetRegionAtPosition(creature->GetPosition());
    if (enemyRegion != creatureRegion) return false;
    return true;
}
int main()
{
    CThing e = { {1, 0, 0}, 10 }, c = { {2, 0, 0}, 9 }; CAIBrain b; b.Creature = &c; CAIStateGroup_MeleeCombatNearby2 g; g.PBrain = &b; g.PEnemy.m_pItem = &e;
    if (!g.IsStillRunnable(true)) { std::printf("0093db10_TEST FAIL\n"); return 1; }
    c.region = 5; if (g.IsStillRunnable(true)) { std::printf("0093db10_TEST FAIL\n"); return 1; }
    c.region = 9; g_baseOk = false; if (g.IsStillRunnable(true)) { std::printf("0093db10_TEST FAIL\n"); return 1; }
    std::printf("0093db10_TEST PASS\n"); return 0;
}