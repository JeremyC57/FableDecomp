// behaviour test: CQ_EndGameBossBattleScript::JackOfBladesBossFightCutscene -- take scripted control of the hero, then of EndGameSister
// (retrying each frame), teleport the sister to MK_JACK1_HOLDJACK and hide her, run CS_JACK_BOSS_INTRO with "Hero" mapped inside a paused
// movie, then teleport her to MK_END1_SISTER and show her; a terminate during either wait returns at once
#include <stdio.h>
#include <string.h>
static int g_frames, g_endAt, g_sisterReadyAt, g_heroTries, g_sisterTries, g_paused, g_macro, g_drawable, g_mapped;
static char g_lastMarker[32];
struct Script {
    bool IsActiveThreadTerminating() const { return g_frames >= g_endAt; }
    void NewScriptFrame() { ++g_frames; }
    bool StartHero() { ++g_heroTries; return g_frames >= 1; }
    bool StartSister() { ++g_sisterTries; return g_frames >= g_sisterReadyAt; }
    void Teleport(const char* marker) { strcpy(g_lastMarker, marker); }
    void JackOfBladesBossFightCutscene();
};
void Script::JackOfBladesBossFightCutscene()
{
    while (!StartHero()) { NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
    if (IsActiveThreadTerminating()) return;
    while (!StartSister()) { NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
    if (IsActiveThreadTerminating()) return;
    Teleport("MK_JACK1_HOLDJACK");
    g_drawable = 0;
    g_mapped = 1;
    {
        g_paused = 1;
        g_macro = 1;
        Teleport("MK_END1_SISTER");
        g_drawable = 1;
        g_paused = 0;
    }
}
static void Run(int endAt, int sisterAt)
{
    g_frames = 0; g_endAt = endAt; g_sisterReadyAt = sisterAt; g_heroTries = g_sisterTries = g_paused = g_macro = g_mapped = 0; g_drawable = -1;
    g_lastMarker[0] = 0;
    Script s; s.JackOfBladesBossFightCutscene();
}
int main()
{
    Run(100, 4);
    if (g_macro != 1 || g_drawable != 1 || g_paused != 0 || strcmp(g_lastMarker, "MK_END1_SISTER") != 0 || g_sisterTries != 4 || g_mapped != 1)
        { printf("00d321c0_TEST FAIL\n"); return 1; }
    Run(3, 10);
    if (g_macro != 0 || g_lastMarker[0] != 0 || g_drawable != -1) { printf("00d321c0_TEST FAIL\n"); return 1; }
    printf("00d321c0_TEST PASS\n");
    return 0;
}