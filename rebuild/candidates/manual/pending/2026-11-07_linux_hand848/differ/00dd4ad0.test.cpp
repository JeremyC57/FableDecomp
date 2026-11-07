// behaviour test: CPrisonerAccomplice::Main -- talk scenes chosen by the master data (KEY / HINT2 / HINT3 / HINT4_01+02), release when the
// hero uses the cell door (persistent, morality, ReleasedAndFollowing), then COURTYARD comments every 30 s until the courtyard loads, then
// GUARDS comments until the thread ends
#include <stdio.h>
#include <string.h>
static int g_frames, g_endAt, g_talkAt, g_doorAt, g_courtyardAt, g_timer, g_comments[2], g_lines[6], g_morality, g_persistent, g_paused;
struct Master { long PrisonRaceNumber; bool PrisonRaceWonByHero, PrisonKeyStolenByHero; };
struct Accomplice {
    Master* PMasterData; bool ReleasedAndFollowing; bool CutsceneFinished;
    bool T() const { return g_frames >= g_endAt; }
    void NSF() { ++g_frames; if (g_timer > 0) --g_timer; }
    void Say(int line) { ++g_lines[line]; }
    void Main();
};
void Accomplice::Main()
{
    NSF(); if (T()) return;
    while (!CutsceneFinished) { NSF(); if (T()) return; }
    while (!ReleasedAndFollowing) {
        NSF(); if (T()) return;
        if (g_frames == g_talkAt) {
            g_paused = 1;
            if (PMasterData->PrisonKeyStolenByHero) Say(0);
            else if (PMasterData->PrisonRaceWonByHero) Say(1);
            else if (PMasterData->PrisonRaceNumber > 1) Say(2);
            else { Say(3); Say(4); }
            g_paused = 0;
        }
        if (g_frames >= g_doorAt) { Say(0); g_persistent = 1; g_morality += 5; ReleasedAndFollowing = true; }
    }
    if (T()) return;
    while (g_frames < g_courtyardAt) {
        NSF(); if (T()) return;
        if (g_timer == 0) { ++g_comments[0]; g_timer = 30; }
    }
    if (T()) return;
    while (!T()) {
        if (g_timer == 0) { ++g_comments[1]; g_timer = 30; }
        NSF();
    }
}
static void Run(Master m, int talkAt, int doorAt, int courtyardAt, int endAt)
{
    g_frames = 0; g_endAt = endAt; g_talkAt = talkAt; g_doorAt = doorAt; g_courtyardAt = courtyardAt; g_timer = 0;
    memset(g_comments, 0, sizeof(g_comments)); memset(g_lines, 0, sizeof(g_lines)); g_morality = g_persistent = g_paused = 0;
    Accomplice a; a.PMasterData = &m; a.ReleasedAndFollowing = false; a.CutsceneFinished = true; a.Main();
}
int main()
{
    Master m = { 0, false, false };
    Run(m, 3, 5, 70, 200);
    if (g_lines[3] != 1 || g_lines[4] != 1 || g_lines[0] != 1 || g_morality != 5 || !g_persistent || g_comments[0] != 3 || g_comments[1] != 4 || g_paused)
        { printf("00dd4ad0_TEST FAIL\n"); return 1; }
    m.PrisonKeyStolenByHero = true;
    Run(m, 3, 5, 70, 200);
    if (g_lines[0] != 2 || g_lines[3] != 0) { printf("00dd4ad0_TEST FAIL\n"); return 1; }
    m.PrisonKeyStolenByHero = false; m.PrisonRaceNumber = 2;
    Run(m, 3, 5, 70, 200);
    if (g_lines[2] != 1) { printf("00dd4ad0_TEST FAIL\n"); return 1; }
    Run(m, 3, 500, 70, 20);
    if (g_morality != 0 || g_comments[0] != 0) { printf("00dd4ad0_TEST FAIL\n"); return 1; }
    printf("00dd4ad0_TEST PASS\n");
    return 0;
}
