// Behaviour test for CTCMultiArrow::CTCMultiArrow @ 0x007CC7D0: counters cleared, empty BowEffect, Init() runs last.
#include <cstdio>
struct CThing { long id; };
struct CBaseIntelligentPointer {
    CThing* m_pItem;
    CBaseIntelligentPointer() : m_pItem((CThing*)1) {}
    void SetPItem(CThing* item) { m_pItem = item; }
};
struct CIntelligentPointer : CBaseIntelligentPointer { CIntelligentPointer(CThing* item) { SetPItem(item); } };
struct CTCBase { CThing* ParentThing; CTCBase(CThing* owner) : ParentThing(owner) {} virtual ~CTCBase() {} };
struct CTCMultiArrow : CTCBase {
    const void* PDef;
    long SpellLevel; bool RemovedEffect;
    long CreateAmmoEffectCount, CreateAmmoEffectDelay, CreateAmmoEffectTimer;
    void* AmmoFirst; void* AmmoLast; void* AmmoEnd;
    CIntelligentPointer BowEffect;
    bool CutSceneCancelled;
    int m_initCalls; bool m_initSawClearedState;
    CTCMultiArrow(CThing* owner)
        : CTCBase(owner), PDef(0), SpellLevel(0), RemovedEffect(false), CreateAmmoEffectCount(0), CreateAmmoEffectDelay(0),
          CreateAmmoEffectTimer(0), AmmoFirst(0), AmmoLast(0), AmmoEnd(0), BowEffect(0), CutSceneCancelled(false), m_initCalls(0)
    { Init(); }
    void Init() { ++m_initCalls; m_initSawClearedState = BowEffect.m_pItem == 0 && !CutSceneCancelled && CreateAmmoEffectTimer == 0; }
};
int main()
{
    CThing t; t.id = 5;
    CTCMultiArrow m(&t);
    if (m.m_initCalls != 1 || !m.m_initSawClearedState || m.PDef != 0 || m.SpellLevel != 0 || m.AmmoFirst != 0 || m.BowEffect.m_pItem != 0)
    { std::printf("007cc7d0_TEST FAIL\n"); return 1; }
    std::printf("007cc7d0_TEST PASS\n");
    return 0;
}
