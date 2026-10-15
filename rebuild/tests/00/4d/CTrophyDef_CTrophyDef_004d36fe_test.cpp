#include <cstdio>
// Behaviour test for CTrophyDef::CTrophyDef (0x004D36FE): base first, the def string = -1, the seven
// sound-name strings constructed in declaration order.
static int g_order[16]; static int g_n = 0;
struct CSubDefClassBase { virtual void v00() { } CSubDefClassBase() { g_order[g_n++] = 100; } };
struct CDefString { long TablePos; CDefString() : TablePos(-1) { } };
struct CCharString { void* m_pStringData; int id; CCharString() : m_pStringData(0) { id = g_n; g_order[g_n++] = 1; } };
struct CTrophyDef : CSubDefClassBase {
    float MinigameInitialTimerSecs, TimeBonusPerWitness; long RenownGainedPerWitnessAhead;
    bool EvilTrophy, FakeTrophy; long EmoteIconForAvailableWitness;
    CDefString ShowTrophyAnimName; float GameDebugTextScale, SecsDelayPerGainedWitnessSound;
    CCharString SoundStartGame, SoundShowTrophy, SoundGainedWitness, SoundOutOfTime,
                SoundGotAllWitnesses, SoundEndNewHighScore, SoundEndNoNewScore;
    CTrophyDef() { }
};
int main()
{
    CTrophyDef d;
    if (g_n != 8 || g_order[0] != 100 || d.ShowTrophyAnimName.TablePos != -1) { std::printf("004d36fe_TEST FAIL\n"); return 1; }
    if (d.SoundStartGame.id != 1 || d.SoundEndNoNewScore.id != 7 || d.SoundEndNoNewScore.m_pStringData != 0) { std::printf("004d36fe_TEST FAIL\n"); return 1; }
    std::printf("004d36fe_TEST PASS\n");
    return 0;
}