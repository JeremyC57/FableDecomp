#include <cstdio>
#include <cstring>
struct CThingCreatureBase { int id; };
struct CThing { unsigned char _pad[0x91]; unsigned char m_flags91; };
static bool g_facing; static CThingCreatureBase* g_c; static CThing* g_t;
bool __fastcall CompareActorRotations(CThingCreatureBase* creature, CThing* target) { g_c = creature; g_t = target; return g_facing; }
struct CBaseIntelligentPointer { virtual void v00() {} CThing* m_pItem; CThing* GetPItem() const { return m_pItem; } };
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p) { if (p.GetPItem() != 0) { if (!(p.GetPItem()->m_flags91 & 1)) return true; } return false; }
struct CCombatActionBase { virtual void v00() {} unsigned char _pad04[4]; CBaseIntelligentPointer PCreature; unsigned char _pad10[0xa8 - 0x10]; CBaseIntelligentPointer PTarget; unsigned char _padb0[0x114 - 0xb0]; };

struct CCreatureAction_BanditKingStuck : CCombatActionBase { bool IsToCauseRecoil() const; };
bool CCreatureAction_BanditKingStuck::IsToCauseRecoil() const
{
    if (IS_THING_ALIVE(PTarget)) {
        CThingCreatureBase* creature = (CThingCreatureBase*)PCreature.GetPItem();
        if (CompareActorRotations(creature, PTarget.GetPItem()))
            return false;
    }
    return true;
}
int main()
{
    CThingCreatureBase c; CThing t; std::memset(&t, 0, sizeof(t)); CCreatureAction_BanditKingStuck a; std::memset(&a, 0, sizeof(a)); a.PCreature.m_pItem = (CThing*)&c; a.PTarget.m_pItem = 0;
    g_facing = true;
    bool r1 = a.IsToCauseRecoil();                       // no target -> true
    a.PTarget.m_pItem = &t; t.m_flags91 = 1; bool r2 = a.IsToCauseRecoil();   // dead target -> true
    t.m_flags91 = 0; g_facing = false; bool r3 = a.IsToCauseRecoil();         // not facing -> true
    g_facing = true; bool r4 = a.IsToCauseRecoil();                           // facing -> false
    bool r5 = true;
    if (r1 != true || r2 != true || r3 != true || r4 != false || r5 != true || g_c != &c || g_t != &t) { std::printf("008c80e0_TEST FAIL\n"); return 1; }
    std::printf("008c80e0_TEST PASS\n");
    return 0;
}
