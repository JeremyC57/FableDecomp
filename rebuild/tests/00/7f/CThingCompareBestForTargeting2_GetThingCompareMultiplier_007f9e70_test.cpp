// behaviour test for CThingCompare_BestForTargeting2::GetThingCompareMultiplier (0x007F9E70)
#include <cstdio>
struct CTCInterface9 { unsigned int m_flags; };
struct CThing { bool Dead; bool has9; CTCInterface9* tc9; bool enemy;
    bool GetInterface(CTCInterface9** out, int) { if (has9) { *out = tc9; return true; } return false; } };
static bool AreThingsEnemies(CThing*, CThing* b) { return b->enemy; }
struct CThingCompare_BestForTargeting2 {
    CThing* Hero; float EnemyPreferenceFactor;
    float GetThingCompareMultiplier(CThing* thing) const
    {
        if (!(thing && !thing->Dead)) return 0.0f;
        CTCInterface9* tc;
        if (thing->GetInterface(&tc, 9) && (tc->m_flags & 4)) return 0.1f;
        if (AreThingsEnemies(Hero, thing)) return EnemyPreferenceFactor;
        return 1.0f;
    }
};
int main()
{
    CThing hero; CTCInterface9 c; c.m_flags = 4;
    CThing t; t.Dead = false; t.has9 = true; t.tc9 = &c; t.enemy = true;
    CThingCompare_BestForTargeting2 cmp; cmp.Hero = &hero; cmp.EnemyPreferenceFactor = 3.0f;
    int bad = cmp.GetThingCompareMultiplier(0) != 0.0f;
    if (cmp.GetThingCompareMultiplier(&t) != 0.1f) bad = 1;
    c.m_flags = 2; if (cmp.GetThingCompareMultiplier(&t) != 3.0f) bad = 1;
    t.enemy = false; if (cmp.GetThingCompareMultiplier(&t) != 1.0f) bad = 1;
    t.Dead = true; if (cmp.GetThingCompareMultiplier(&t) != 0.0f) bad = 1;
    if (bad) { std::printf("007f9e70_TEST FAIL\n"); return 1; }
    std::printf("007f9e70_TEST PASS\n");
    return 0;
}