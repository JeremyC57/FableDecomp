// Behaviour test for 0x00F2D200 CFeatDef operator=: every feat field copied, base copied first.
#include <cstdio>
static int g_baseCopies = 0;
struct CDefClassBase {
    int m_base;
    CDefClassBase() : m_base(0) {}
    CDefClassBase(const CDefClassBase& o) : m_base(o.m_base) { ++g_baseCopies; }
    CDefClassBase& operator=(const CDefClassBase& o) { m_base = o.m_base; ++g_baseCopies; return *this; }
};
struct CDefString { int TablePos; };
struct CFeatDef : CDefClassBase {
    CDefString FeatName, Verb; float TimeLimit; int TargetNumber, GoldReward, XPReward; CDefString ItemReward;
    bool NoBlocking; int KN_AttackType; bool KN_Perfect; int KN_CreatureType; CDefString GF_FromRegion, GF_ToRegion;
    bool GF_NoTeleporting; CDefString CO_ItemName;
    CFeatDef() {}
    CFeatDef(const CFeatDef& other)
        : CDefClassBase(other), FeatName(other.FeatName), Verb(other.Verb), TimeLimit(other.TimeLimit),
          TargetNumber(other.TargetNumber), GoldReward(other.GoldReward), XPReward(other.XPReward), ItemReward(other.ItemReward),
          NoBlocking(other.NoBlocking), KN_AttackType(other.KN_AttackType), KN_Perfect(other.KN_Perfect),
          KN_CreatureType(other.KN_CreatureType), GF_FromRegion(other.GF_FromRegion), GF_ToRegion(other.GF_ToRegion),
          GF_NoTeleporting(other.GF_NoTeleporting), CO_ItemName(other.CO_ItemName) {}
    CFeatDef& operator=(const CFeatDef& other)
    {
        CDefClassBase::operator=(other);
        FeatName = other.FeatName; Verb = other.Verb; TimeLimit = other.TimeLimit; TargetNumber = other.TargetNumber;
        GoldReward = other.GoldReward; XPReward = other.XPReward; ItemReward = other.ItemReward; NoBlocking = other.NoBlocking;
        KN_AttackType = other.KN_AttackType; KN_Perfect = other.KN_Perfect; KN_CreatureType = other.KN_CreatureType;
        GF_FromRegion = other.GF_FromRegion; GF_ToRegion = other.GF_ToRegion; GF_NoTeleporting = other.GF_NoTeleporting;
        CO_ItemName = other.CO_ItemName;
        return *this;
    }
};
int main()
{
    CFeatDef a;
    a.m_base = 7; a.FeatName.TablePos = 1; a.Verb.TablePos = 2; a.TimeLimit = 2.5f; a.TargetNumber = 3; a.GoldReward = 4;
    a.XPReward = 5; a.ItemReward.TablePos = 6; a.NoBlocking = true; a.KN_AttackType = 8; a.KN_Perfect = true;
    a.KN_CreatureType = 9; a.GF_FromRegion.TablePos = 10; a.GF_ToRegion.TablePos = 11; a.GF_NoTeleporting = true;
    a.CO_ItemName.TablePos = 12;
    CFeatDef b; b = a;
    if (g_baseCopies != 1 || b.m_base != 7 || b.FeatName.TablePos != 1 || b.Verb.TablePos != 2 || b.TimeLimit != 2.5f ||
        b.TargetNumber != 3 || b.GoldReward != 4 || b.XPReward != 5 || b.ItemReward.TablePos != 6 || !b.NoBlocking ||
        b.KN_AttackType != 8 || !b.KN_Perfect || b.KN_CreatureType != 9 || b.GF_FromRegion.TablePos != 10 ||
        b.GF_ToRegion.TablePos != 11 || !b.GF_NoTeleporting || b.CO_ItemName.TablePos != 12) {
        std::printf("00f2d200_TEST FAIL\n"); return 1;
    }
    std::printf("00f2d200_TEST PASS\n");
    return 0;
}