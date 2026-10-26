// behaviour test for 0x00700940 COpinionReactionManager::LogReaction
#include <cstdio>
static long g_frame = 500;
class CThing;
class CWorld { public: static long GetWorldFrame() { return g_frame; } };
static CWorld g_world;
class CTCCreatureOpinionOfHero { public:
    long LastOpinionReactionFrame;
    CWorld* GetWorld() { return &g_world; }
};
class CThing { public:
    CTCCreatureOpinionOfHero* tc;
    CTCCreatureOpinionOfHero* GetOpinionOfHeroTC() { return tc; }
};
class CReactionFrequencyTraits { public:
    int calls; bool lastFlag; const CThing* lastThing;
    virtual void LogReaction(const CThing* creature, bool fromScript) { ++calls; lastFlag = fromScript; lastThing = creature; }
    virtual ~CReactionFrequencyTraits() {}
};
class COpinionReactionManagerDef { public: CReactionFrequencyTraits** FrequencyTraits; CReactionFrequencyTraits** FrequencyTraitsHigh; };
class COpinionReactionManager { public:
    const COpinionReactionManagerDef* PDef;
    void LogReaction(long reaction, CThing* creature, bool fromScript)
    {
        CReactionFrequencyTraits* traits = PDef->FrequencyTraits[reaction];
        if (traits) traits->LogReaction(creature, fromScript);
        CReactionFrequencyTraits* highTraits = PDef->FrequencyTraitsHigh[reaction];
        if (highTraits) highTraits->LogReaction(creature, fromScript);
        if (!fromScript) {
            CTCCreatureOpinionOfHero* opinion = creature->GetOpinionOfHeroTC();
            opinion->LastOpinionReactionFrame = opinion->GetWorld()->GetWorldFrame();
        }
    }
};
int main()
{
    CReactionFrequencyTraits a, b; a.calls = b.calls = 0;
    CReactionFrequencyTraits* normal[3] = { 0, &a, &a };
    CReactionFrequencyTraits* high[3] = { 0, 0, &b };
    COpinionReactionManagerDef def = { normal, high };
    COpinionReactionManager m; m.PDef = &def;
    CTCCreatureOpinionOfHero tc = { 0 }; CThing creature = { &tc };
    m.LogReaction(1, &creature, true);
    int bad = a.calls != 1 || b.calls != 0 || !a.lastFlag || a.lastThing != &creature || tc.LastOpinionReactionFrame != 0;
    m.LogReaction(2, &creature, false);
    bad |= a.calls != 2 || b.calls != 1 || b.lastFlag || tc.LastOpinionReactionFrame != 500;
    g_frame = 600;
    m.LogReaction(0, &creature, false);
    bad |= a.calls != 2 || b.calls != 1 || tc.LastOpinionReactionFrame != 600;
    if (bad) { std::printf("00700940_TEST FAIL\n"); return 1; }
    std::printf("00700940_TEST PASS\n");
    return 0;
}