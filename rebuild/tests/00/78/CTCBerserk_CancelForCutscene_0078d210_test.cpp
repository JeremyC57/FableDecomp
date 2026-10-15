// Behaviour test for 0x0078D210 CTCBerserk::CancelForCutscene: the first call marks IsInCutscene, finishes the
// parent's action, cleans up the ability (cutscene) and removes TC 0x84; a second call does nothing.
#include <cstdio>

static int g_log[8];
static int g_n;
static long g_removed;
static bool g_cleanupArg;
struct CThing { void RemoveTC(long type) { g_log[g_n++] = 3; g_removed = type; } };
struct CThingCreatureBase : CThing { void FinishCurrentAction() { g_log[g_n++] = 1; } };
struct CTCBerserk {
    CThingCreatureBase* ParentThing;
    bool                IsInCutscene;
    void CleanupAbility(bool cutscene) { g_log[g_n++] = 2; g_cleanupArg = cutscene; }
    void CancelForCutscene()
    {
        if (!IsInCutscene) {
            CThingCreatureBase* parent = ParentThing;
            IsInCutscene = true;
            parent->FinishCurrentAction();
            CleanupAbility(true);
            parent->RemoveTC(0x84);
        }
    }
};

int main()
{
    CThingCreatureBase creature;
    CTCBerserk b; b.ParentThing = &creature; b.IsInCutscene = false;
    g_n = 0;
    b.CancelForCutscene();
    if (!b.IsInCutscene || g_n != 3 || g_log[0] != 1 || g_log[1] != 2 || g_log[2] != 3 || !g_cleanupArg || g_removed != 0x84) { std::printf("0078d210_TEST FAIL\n"); return 1; }
    g_n = 0;
    b.CancelForCutscene();
    if (g_n != 0) { std::printf("0078d210_TEST FAIL\n"); return 1; }
    std::printf("0078d210_TEST PASS\n");
    return 0;
}