// Behaviour test for CAIStateGroup_MinionKidGroupTag::Update (0x00934870): per pass the world-seeded GFFloatRandom(100) below 5
// ends the game (Terminate); otherwise the tagger tags and the others evade; repeat while the do-nothing action returns 4.
#include <stdio.h>
#include <math.h>
static unsigned long g_seed = 1;
static int g_terminates = 0, g_tags = 0, g_evades = 0, g_doNothingLeft = 0;
static long GFROR13(unsigned long v) { return (long)((v >> 13) | (v << 19)); }
static float GFFloatRandom(float range, unsigned long* seed)
{
    *seed = *seed * 0x24A1 + 0x24DF;
    *seed = GFROR13(*seed);
    return (float)(fmod((double)*seed, range * 65536.0) / 65536.0);
}
class CThingAICreature { public: int Dummy; };
class CTCCreatureGroupTag { public:
    const CThingAICreature* Tagger;
    const CThingAICreature* GetTagger() const { return Tagger; }
};
class CAIStateGroup_MinionKidGroupTag { public:
    CThingAICreature* Creature; CTCCreatureGroupTag* PTCGroupTag;
    CThingAICreature& GetCreature() const { return *Creature; }
    unsigned long* GetPWorldSeed() const { return &g_seed; }
    void Terminate() { g_terminates++; }
    int AIAction_DoNothing() { return g_doNothingLeft-- > 0 ? 4 : 1; }
    void TagTarget() { g_tags++; }
    void EvadeTarget() { g_evades++; }
    void Update()
    {
        do {
            if (GFFloatRandom(100.0f, GetPWorldSeed()) < 5.0f) { Terminate(); return; }
            if (PTCGroupTag->GetTagger() == &GetCreature()) TagTarget(); else EvadeTarget();
        } while (AIAction_DoNothing() == 4);
        Terminate();
    }
};
int main()
{
    unsigned long s = 1;                                 // the generator stays inside [0, range)
    for (int i = 0; i < 1000; ++i) { float r = GFFloatRandom(100.0f, &s); if (r < 0.0f || r >= 100.0f) { printf("00934870_TEST FAIL\n"); return 1; } }
    CThingAICreature kid, other; CTCCreatureGroupTag tag; tag.Tagger = &kid;
    CAIStateGroup_MinionKidGroupTag group; group.Creature = &kid; group.PTCGroupTag = &tag;
    // find a seed whose next draw is >= 5 for the next three draws, then whose next draw is < 5
    unsigned long seed = 0; int found = 0;
    for (unsigned long cand = 1; cand < 100000 && !found; ++cand) {
        unsigned long t = cand; int ok = 1;
        for (int k = 0; k < 3; ++k) if (GFFloatRandom(100.0f, &t) < 5.0f) ok = 0;
        if (ok && GFFloatRandom(100.0f, &t) < 5.0f) { seed = cand; found = 1; }
    }
    if (!found) { printf("00934870_TEST FAIL\n"); return 1; }
    g_seed = seed; g_doNothingLeft = 10;                 // keeps looping until the 4th draw ends the game
    group.Update();
    if (g_tags != 3 || g_evades != 0 || g_terminates != 1) { printf("00934870_TEST FAIL\n"); return 1; }
    // not the tagger: evades; the do-nothing action stops the loop after the first pass
    g_seed = seed; g_doNothingLeft = 0; tag.Tagger = &other;
    group.Update();
    if (g_tags != 3 || g_evades != 1 || g_terminates != 2) { printf("00934870_TEST FAIL\n"); return 1; }
    printf("00934870_TEST PASS\n");
    return 0;
}