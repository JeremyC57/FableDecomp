// Behaviour test for CTCSummonSpell::CancelForCutscene (0079a660): toggles the cut-scene mark once and moves a live minion
// into limbo.
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
    void CancelForCutscene();
};

void CTCSummonSpell::CancelForCutscene()
{
    if (!CutSceneCancelled) {
        CutSceneCancelled = true;
        if (Minion.GetPItem() && !(Minion.GetPItem()->m_thingFlags & 1))
            Minion.GetPItem()->SetInLimbo(true);
    }
}

int main()
{
    CThing minion; minion.m_thingFlags = 0;
    CTCSummonSpell s; s.Minion.m_pItem = 0; s.CutSceneCancelled = false;
    s.CancelForCutscene();                                    // no minion: only the mark changes
    if (s.CutSceneCancelled != true || g_limboCalls) { std::printf("0079a660_TEST FAIL\n"); return 1; }
    s.Minion.m_pItem = &minion;
    s.CancelForCutscene();                                    // mark already in place: nothing
    if (g_limboCalls) { std::printf("0079a660_TEST FAIL\n"); return 1; }
    s.CutSceneCancelled = false;
    s.CancelForCutscene();
    if (s.CutSceneCancelled != true || g_limboCalls != 1 || g_limboArg != true) { std::printf("0079a660_TEST FAIL\n"); return 1; }
    s.CutSceneCancelled = false; minion.m_thingFlags = 1;
    s.CancelForCutscene();                                    // killed minion untouched
    if (g_limboCalls != 1) { std::printf("0079a660_TEST FAIL\n"); return 1; }
    std::printf("0079a660_TEST PASS\n");
    return 0;
}
