// Behaviour test for CTCSummonSpell::ResumeForCutscene (0079a6a0): toggles the cut-scene mark once and moves a live minion
// out of limbo.
#include <cstdio>

static int g_limboCalls = 0;
static bool g_limboArg = false;

struct CThing {
    unsigned char m_thingFlags;
    void SetInLimbo(bool inLimbo) { ++g_limboCalls; g_limboArg = inLimbo; }
};

struct CBaseIntelligentPointer {
    CThing* m_pItem;
    CThing* GetPItem() const { return m_pItem; }
};

struct CTCSummonSpell {
    bool                    JustReleased;
    CBaseIntelligentPointer Minion;
    bool                    CutSceneCancelled;
    void ResumeForCutscene();
};

void CTCSummonSpell::ResumeForCutscene()
{
    if (CutSceneCancelled) {
        CutSceneCancelled = false;
        if (Minion.GetPItem() && !(Minion.GetPItem()->m_thingFlags & 1))
            Minion.GetPItem()->SetInLimbo(false);
    }
}

int main()
{
    CThing minion; minion.m_thingFlags = 0;
    CTCSummonSpell s; s.Minion.m_pItem = 0; s.CutSceneCancelled = true;
    s.ResumeForCutscene();                                    // no minion: only the mark changes
    if (s.CutSceneCancelled != false || g_limboCalls) { std::printf("0079a6a0_TEST FAIL\n"); return 1; }
    s.Minion.m_pItem = &minion;
    s.ResumeForCutscene();                                    // mark already in place: nothing
    if (g_limboCalls) { std::printf("0079a6a0_TEST FAIL\n"); return 1; }
    s.CutSceneCancelled = true;
    s.ResumeForCutscene();
    if (s.CutSceneCancelled != false || g_limboCalls != 1 || g_limboArg != false) { std::printf("0079a6a0_TEST FAIL\n"); return 1; }
    s.CutSceneCancelled = true; minion.m_thingFlags = 1;
    s.ResumeForCutscene();                                    // killed minion untouched
    if (g_limboCalls != 1) { std::printf("0079a6a0_TEST FAIL\n"); return 1; }
    std::printf("0079a6a0_TEST PASS\n");
    return 0;
}
