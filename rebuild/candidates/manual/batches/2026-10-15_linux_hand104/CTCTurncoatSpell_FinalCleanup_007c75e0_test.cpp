#include <cstdio>
struct CThing { bool dead; int fades; void FadeOutEffect() { ++fades; } bool IsDead() const { return dead; } };
struct CBaseIntelligentPointer { CThing* m_pItem; CThing* GetPItem() const { return m_pItem; } };
static bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) { if (!p.GetPItem()->IsDead()) return true; }
    return false;
}
struct CTCTurncoatSpell {
    CBaseIntelligentPointer TurncoatingEffectOnMeOutOfRange; int stops;
    void StopTurncoatingEffects() { ++stops; }
    void FinalCleanup()
    {
        if (IS_THING_ALIVE(TurncoatingEffectOnMeOutOfRange))
            TurncoatingEffectOnMeOutOfRange.GetPItem()->FadeOutEffect();
        StopTurncoatingEffects();
    }
};
int main()
{
    CThing fx; fx.dead = false; fx.fades = 0;
    CTCTurncoatSpell s; s.stops = 0; s.TurncoatingEffectOnMeOutOfRange.m_pItem = &fx;
    s.FinalCleanup();
    if (fx.fades != 1 || s.stops != 1) { std::printf("007c75e0_TEST FAIL\n"); return 1; }
    fx.dead = true; s.FinalCleanup();
    if (fx.fades != 1 || s.stops != 2) { std::printf("007c75e0_TEST FAIL\n"); return 1; }
    s.TurncoatingEffectOnMeOutOfRange.m_pItem = 0; s.FinalCleanup();
    if (s.stops != 3) { std::printf("007c75e0_TEST FAIL\n"); return 1; }
    std::printf("007c75e0_TEST PASS\n");
    return 0;
}
