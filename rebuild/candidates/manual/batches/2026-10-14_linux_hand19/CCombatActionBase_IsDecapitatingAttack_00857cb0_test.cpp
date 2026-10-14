#include <cstdio>

// Model of CCombatActionBase::IsDecapitatingAttack @ 0x00857CB0

struct CTCBase { int id; };
enum ETCInterfaceType { TC_INTERFACE_HERO = 0x29, TC_INTERFACE_BERSERK = 0x84 };
struct CInterfacePair { ETCInterfaceType first; CTCBase* second; };
struct CInterfaceMap {
    CInterfacePair* m_pFirst; CInterfacePair* m_pLast;
    CInterfacePair* LowerBound(const long& key) { CInterfacePair* it = m_pFirst; while (it != m_pLast && (long)it->first < key) ++it; return it; }
    CInterfacePair* Find(ETCInterfaceType key) { CInterfacePair* it = LowerBound(key); if (it == m_pLast || key < it->first) return m_pLast; return it; }
};

static bool g_should = false; static int g_shouldCalls = 0;
struct CTCBerserk { int id; bool ShouldAttackDecapitate() { ++g_shouldCalls; return g_should; } };
struct CThingCreatureBase { unsigned char _pad[0x30]; unsigned long Flags30; unsigned char _pad34[0x44 - 0x34]; CInterfaceMap Interfaces;
    CTCBerserk* GetBerserkInterface() { return reinterpret_cast<CTCBerserk*>(Interfaces.Find(TC_INTERFACE_BERSERK)->second); } };
struct CMeleeCombatAbilityDef { unsigned char _pad[0x5c]; bool CanDecapitate; };
struct CBaseIntelligentPointer { virtual void v00() {} CThingCreatureBase* m_pItem; CThingCreatureBase* GetPItem() const { return m_pItem; } };
struct CCombatActionBase {
    virtual void v00() {} unsigned char _pad04[4]; CBaseIntelligentPointer PCreature; unsigned char _pad10[0xb8 - 0x10]; CMeleeCombatAbilityDef* PMeleeAbilityDef; unsigned char _padbc[0x114 - 0xbc];
    bool IsDecapitatingAttack() const {
        if (PMeleeAbilityDef != 0 && PMeleeAbilityDef->CanDecapitate) {
            CThingCreatureBase* creature = PCreature.GetPItem();
            if (creature->Flags30 & 0x10) return creature->GetBerserkInterface()->ShouldAttackDecapitate();
            return true;
        }
        return false;
    }
};
int main()
{
    CTCBerserk berserk; CInterfacePair pairs[2]; pairs[0].first = (ETCInterfaceType)5; pairs[0].second = 0; pairs[1].first = TC_INTERFACE_BERSERK; pairs[1].second = (CTCBase*)&berserk;
    CThingCreatureBase c; c.Interfaces.m_pFirst = pairs; c.Interfaces.m_pLast = pairs + 2; c.Flags30 = 0;
    CMeleeCombatAbilityDef def; def.CanDecapitate = false;
    CCombatActionBase a; a.PCreature.m_pItem = &c; a.PMeleeAbilityDef = 0;
    if (a.IsDecapitatingAttack()) { std::printf("00857cb0_TEST FAIL\n"); return 1; }
    a.PMeleeAbilityDef = &def;
    if (a.IsDecapitatingAttack()) { std::printf("00857cb0_TEST FAIL\n"); return 1; }
    def.CanDecapitate = true;
    if (!a.IsDecapitatingAttack() || g_shouldCalls != 0) { std::printf("00857cb0_TEST FAIL\n"); return 1; }
    c.Flags30 = 0x10; g_should = false;
    if (a.IsDecapitatingAttack() || g_shouldCalls != 1) { std::printf("00857cb0_TEST FAIL\n"); return 1; }
    g_should = true;
    if (!a.IsDecapitatingAttack() || g_shouldCalls != 2) { std::printf("00857cb0_TEST FAIL\n"); return 1; }
    std::printf("00857cb0_TEST PASS\n"); return 0;
}
