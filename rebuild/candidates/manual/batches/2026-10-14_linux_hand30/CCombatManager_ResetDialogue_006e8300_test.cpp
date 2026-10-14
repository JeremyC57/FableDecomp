#include <cstdio>
#include <cstring>
#include <cmath>

static int g_init = 0;
struct CombatDialogueManager { unsigned char b[0x28]; void Init() { g_init++; } };
struct CCombatManager { unsigned char p[0x30]; float AttackPatternIdleStandardInterval; bool AttacksFromNearestCombatantOnly; unsigned char q[3]; CombatDialogueManager DialogueManager;
    void ResetDialogue() { DialogueManager.Init(); AttackPatternIdleStandardInterval = 0.5f; AttacksFromNearestCombatantOnly = false; } };
int main() {
    CCombatManager m; m.AttackPatternIdleStandardInterval = 9.0f; m.AttacksFromNearestCombatantOnly = true; m.ResetDialogue();
    if (g_init != 1 || m.AttackPatternIdleStandardInterval != 0.5f || m.AttacksFromNearestCombatantOnly) { std::printf("006e8300_TEST FAIL\n"); return 1; }
    std::printf("006e8300_TEST PASS\n"); return 0;
}