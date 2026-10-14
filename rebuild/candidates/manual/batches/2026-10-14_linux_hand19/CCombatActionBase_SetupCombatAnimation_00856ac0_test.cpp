#include <cstdio>

// Model of CCombatActionBase::SetupCombatAnimation @ 0x00856AC0
struct CAnimationEntry { int id; };
struct CAnimationEntryVector { const CAnimationEntry** m_pFirst; const CAnimationEntry** m_pLast; const CAnimationEntry** m_pEnd; const CAnimationEntry* operator[](long i) const { return m_pFirst[i]; } };
static const CAnimationEntryVector* g_listSeen = 0;
struct CThingCreatureBase { long GetAppropriateCombatAnimationFromList(const CAnimationEntryVector& v) { g_listSeen = &v; return 2; } };
struct CBaseIntelligentPointer { virtual void v00() {} CThingCreatureBase* m_pItem; CThingCreatureBase* GetPItem() const { return m_pItem; } };
static const CAnimationEntry* g_entry = 0; static float g_start = 0, g_end = 0;
struct CCombatActionBase {
    virtual void v00() {} unsigned char _pad04[4]; CBaseIntelligentPointer PCreature; unsigned char _pad10[0xf0 - 0x10]; CAnimationEntryVector CombatAnimations; unsigned char _padfc[0x114 - 0xfc];
    void SetAnimation(const CAnimationEntry* e, float s, float en) { g_entry = e; g_start = s; g_end = en; }
    void SetupCombatAnimation() { SetAnimation(CombatAnimations[PCreature.GetPItem()->GetAppropriateCombatAnimationFromList(CombatAnimations)], -1.0f, 1.0f); }
};
int main()
{
    CAnimationEntry e0, e1, e2; const CAnimationEntry* list[3] = { &e0, &e1, &e2 };
    CThingCreatureBase c; CCombatActionBase a; a.PCreature.m_pItem = &c; a.CombatAnimations.m_pFirst = list; a.CombatAnimations.m_pLast = list + 3; a.CombatAnimations.m_pEnd = list + 3;
    a.SetupCombatAnimation();
    if (g_entry != &e2 || g_start != -1.0f || g_end != 1.0f || g_listSeen != &a.CombatAnimations || (sizeof(void*) == 4 && (char*)&a.CombatAnimations - (char*)&a != 0xf0)) { std::printf("00856ac0_TEST FAIL\n"); return 1; }
    std::printf("00856ac0_TEST PASS\n"); return 0;
}
