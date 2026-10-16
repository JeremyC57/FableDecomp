#include <cstdio>
// Behaviour model of CTCWillLightning::ResumeForCutscene (0x007a0400): restart the looping sound when none is playing and the owner has a sound component
static const unsigned long g_InvalidSoundHandle = 0xFFFFFFFFu;
static long g_played;
struct CTCSound { unsigned long PlaySound(long name, bool, long, long, long) { g_played = name; return 42; } };
struct CThing { CTCSound* sound; bool GetInterface(CTCSound** out, long t) { if (t == 16 && sound) { *out = sound; return true; } return false; } };
struct CDef { long LoopingSound; };
struct CSpell { CThing* Owner; const CDef* Def; unsigned long LoopingSound;
    void ResumeForCutscene() {
        if (LoopingSound == g_InvalidSoundHandle) {
            CTCSound* s;
            if (Owner->GetInterface(&s, 16)) LoopingSound = s->PlaySound(Def->LoopingSound, false, 0, 0, -1);
        }
    } };
int main()
{
    CTCSound snd; CThing t = { &snd }, u = { 0 }; CDef d = { 9 };
    CSpell a = { &t, &d, g_InvalidSoundHandle };
    a.ResumeForCutscene();
    if (a.LoopingSound != 42 || g_played != 9) { std::printf("007a0400_TEST FAIL\n"); return 1; }
    g_played = 0; a.ResumeForCutscene();
    CSpell b = { &u, &d, g_InvalidSoundHandle }; b.ResumeForCutscene();
    if (g_played || b.LoopingSound != g_InvalidSoundHandle) { std::printf("007a0400_TEST FAIL\n"); return 1; }
    std::printf("007a0400_TEST PASS\n");
    return 0;
}
