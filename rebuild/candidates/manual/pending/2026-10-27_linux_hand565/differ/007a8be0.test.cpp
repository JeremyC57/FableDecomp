// Behaviour test for CTCBattleCharge::FrameUpdate @ 0x007A8BE0: release handling, pose countdown, exit modes once, removal.
#include <stdio.h>
#include <string.h>
static char g_sound[64]; static int g_removed = 0, g_finished = 0, g_startUp = 0, g_rush = 0, g_suspendOff = 0, g_unfreeze = 0, g_modeRemoved = 0;
struct CTCSpecialAbilities { bool AbilityMutexLock; long CurrentAbility; };
struct CThing { int Type; bool Dead; bool HasSound; CTCSpecialAbilities* Abilities; bool HasModes; };
struct CTCBattleCharge { CThing* ParentThing; bool JustReleased; long SpellLevel; long PoseDelay; bool StartPoseDelay; long ChargeDelayFrameCount; bool CompletedExitModes;
    void FrameUpdate() {
        CThing* parent = ParentThing;
        if (JustReleased) {
            g_finished++;
            if (parent->HasSound) {
                static const char* names[4] = { "HERO_ABILITY_BATTLE_CHARGE_CAST_NO_POWERUP", "HERO_ABILITY_BATTLE_CHARGE_CAST_LEVEL_1",
                    "HERO_ABILITY_BATTLE_CHARGE_CAST_LEVEL_2", "HERO_ABILITY_BATTLE_CHARGE_CAST_LEVEL_3" };
                if (SpellLevel >= 0 && SpellLevel <= 3) strcpy(g_sound, names[SpellLevel]);
            }
            JustReleased = false; g_startUp++;
        } else if (StartPoseDelay) --PoseDelay; else g_rush++;
        if (PoseDelay <= 0) {
            if (!CompletedExitModes) {
                if (parent->Abilities && parent->Abilities->CurrentAbility == 12) { parent->Abilities->AbilityMutexLock = false; parent->Abilities->CurrentAbility = 0; }
                if (!parent->Dead && parent->Type == 1) g_suspendOff++;
                if (parent->HasModes) g_modeRemoved++;
                if (!parent->Dead && parent->Type == 2) g_unfreeze++;
                CompletedExitModes = true;
            }
            --ChargeDelayFrameCount;
            if (parent->Type == 1 || ChargeDelayFrameCount <= 0) g_removed++;
        }
    } };
int main()
{
    CTCSpecialAbilities ab = { true, 12 };
    CThing hero = { 2, false, true, &ab, true };
    CTCBattleCharge c = { &hero, true, 2, 1, true, 2, false };
    c.FrameUpdate();
    if (strcmp(g_sound, "HERO_ABILITY_BATTLE_CHARGE_CAST_LEVEL_2") != 0 || g_startUp != 1 || g_removed != 0) { printf("007a8be0_TEST FAIL\n"); return 1; }
    c.FrameUpdate();   // pose delay 1 -> 0: exit modes, charge delay 2 -> 1
    if (c.PoseDelay != 0 || !c.CompletedExitModes || ab.AbilityMutexLock || g_unfreeze != 1 || g_modeRemoved != 1 || g_removed != 0) { printf("007a8be0_TEST FAIL\n"); return 1; }
    c.StartPoseDelay = false; c.FrameUpdate();
    if (g_rush != 1 || g_unfreeze != 1 || g_removed != 1) { printf("007a8be0_TEST FAIL\n"); return 1; }
    printf("007a8be0_TEST PASS\n");
    return 0;
}
