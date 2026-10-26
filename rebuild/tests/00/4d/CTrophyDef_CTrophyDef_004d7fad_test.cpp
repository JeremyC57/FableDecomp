#include <cstdio>
// Behaviour test for CTrophyDef::~CTrophyDef (0x004D7FAD): the seven strings die last-declared first, then
// the def-pointee layers.
static int g_order[16]; static int g_n = 0;
class CCharString { public: int id; ~CCharString() { g_order[g_n++] = id; } };
class CResource { public: virtual void v00() { } ~CResource() { g_order[g_n++] = 100; } };
class CDefPointeeBase : public CResource { public: ~CDefPointeeBase() { g_order[g_n++] = 50; } };
class CSubDefClassBase : public CDefPointeeBase { public: };
class CTrophyDef : public CSubDefClassBase { public:
    long ShowTrophyAnimName;
    CCharString SoundStartGame, SoundShowTrophy, SoundGainedWitness, SoundOutOfTime,
                SoundGotAllWitnesses, SoundEndNewHighScore, SoundEndNoNewScore;
};
int main()
{
    {
        CTrophyDef d;
        d.SoundStartGame.id = 1; d.SoundShowTrophy.id = 2; d.SoundGainedWitness.id = 3; d.SoundOutOfTime.id = 4;
        d.SoundGotAllWitnesses.id = 5; d.SoundEndNewHighScore.id = 6; d.SoundEndNoNewScore.id = 7;
    }
    int want[9] = { 7, 6, 5, 4, 3, 2, 1, 50, 100 };
    if (g_n != 9) { std::printf("004d7fad_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 9; ++i)
        if (g_order[i] != want[i]) { std::printf("004d7fad_TEST FAIL\n"); return 1; }
    std::printf("004d7fad_TEST PASS\n");
    return 0;
}