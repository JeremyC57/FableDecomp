#include <cstdio>
#include <cstring>
struct CPlayer { unsigned char _pad[0x20a]; unsigned char m_autoFaceTarget; };
static CPlayer g_player;
struct CThingCreatureBase_Calls { CPlayer* GetPlayer() { return &g_player; } };
struct CBaseIntelligentPointer { virtual void v00() { } CThingCreatureBase_Calls* m_pItem; CThingCreatureBase_Calls* GetPItem() const { return m_pItem; } };
struct CCreatureAction_InterruptableMidAttackAutoTurn { virtual void v00() { } unsigned char _pad04[4]; CBaseIntelligentPointer PCreature; bool IsToFaceTarget() const; };
bool CCreatureAction_InterruptableMidAttackAutoTurn::IsToFaceTarget() const { return PCreature.GetPItem()->GetPlayer()->m_autoFaceTarget ? true : false; }
int main()
{
    CThingCreatureBase_Calls creature; CCreatureAction_InterruptableMidAttackAutoTurn a; a.PCreature.m_pItem = &creature;
    g_player.m_autoFaceTarget = 0; if (a.IsToFaceTarget()) { std::printf("0087c900_TEST FAIL\n"); return 1; }
    g_player.m_autoFaceTarget = 0x40; if (a.IsToFaceTarget() != true) { std::printf("0087c900_TEST FAIL\n"); return 1; }
    std::printf("0087c900_TEST PASS\n");
    return 0;
}