// behaviour test for CTrophyDef::operator= (0x004E0474): every member copied, base assignment called, *this returned
#include <cstdio>
static int g_base = 0, g_ops = 0;
class CDefClassBase { public: unsigned char _body[0x25]; CDefClassBase& operator=(const CDefClassBase& o) { ++g_base; for (int i = 0; i < 0x25; ++i) _body[i] = o._body[i]; return *this; } };
class CCharString { public: void* m_pStringData; const CCharString& operator=(const CCharString& o) { ++g_ops; m_pStringData = o.m_pStringData; return *this; } }; class CDefString { public: long TablePos; };
class CTrophyDef : public CDefClassBase { public:
    float MinigameInitialTimerSecs;
    float TimeBonusPerWitness;
    long RenownGainedPerWitnessAhead;
    bool EvilTrophy;
    bool FakeTrophy;
    long EmoteIconForAvailableWitness;
    CDefString ShowTrophyAnimName;
    float GameDebugTextScale;
    float SecsDelayPerGainedWitnessSound;
    CCharString SoundStartGame;
    CCharString SoundShowTrophy;
    CCharString SoundGainedWitness;
    CCharString SoundOutOfTime;
    CCharString SoundGotAllWitnesses;
    CCharString SoundEndNewHighScore;
    CCharString SoundEndNoNewScore;
    CTrophyDef& operator=(const CTrophyDef& other);
};
CTrophyDef& CTrophyDef::operator=(const CTrophyDef& other)
{
    CDefClassBase::operator=(other);
    MinigameInitialTimerSecs = other.MinigameInitialTimerSecs;
    TimeBonusPerWitness = other.TimeBonusPerWitness;
    RenownGainedPerWitnessAhead = other.RenownGainedPerWitnessAhead;
    EvilTrophy = other.EvilTrophy;
    FakeTrophy = other.FakeTrophy;
    EmoteIconForAvailableWitness = other.EmoteIconForAvailableWitness;
    ShowTrophyAnimName = other.ShowTrophyAnimName;
    GameDebugTextScale = other.GameDebugTextScale;
    SecsDelayPerGainedWitnessSound = other.SecsDelayPerGainedWitnessSound;
    SoundStartGame = other.SoundStartGame;
    SoundShowTrophy = other.SoundShowTrophy;
    SoundGainedWitness = other.SoundGainedWitness;
    SoundOutOfTime = other.SoundOutOfTime;
    SoundGotAllWitnesses = other.SoundGotAllWitnesses;
    SoundEndNewHighScore = other.SoundEndNewHighScore;
    SoundEndNoNewScore = other.SoundEndNoNewScore;
    return *this;
}
int main()
{
    CTrophyDef a, b;
    unsigned char* pa = (unsigned char*)&a; unsigned char* pb = (unsigned char*)&b;
    for (unsigned int i = 0; i < sizeof(a); ++i) { pa[i] = (unsigned char)(i * 7 + 3); pb[i] = 0; }
    a.EvilTrophy = true;
    a.FakeTrophy = true;
    CTrophyDef* r = &(b = a);
    int bad = (r != &b) || g_base != 1;
    { const unsigned char* x = (const unsigned char*)&a.MinigameInitialTimerSecs; const unsigned char* y = (const unsigned char*)&b.MinigameInitialTimerSecs; for (unsigned int i = 0; i < sizeof(a.MinigameInitialTimerSecs); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.TimeBonusPerWitness; const unsigned char* y = (const unsigned char*)&b.TimeBonusPerWitness; for (unsigned int i = 0; i < sizeof(a.TimeBonusPerWitness); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.RenownGainedPerWitnessAhead; const unsigned char* y = (const unsigned char*)&b.RenownGainedPerWitnessAhead; for (unsigned int i = 0; i < sizeof(a.RenownGainedPerWitnessAhead); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.EvilTrophy; const unsigned char* y = (const unsigned char*)&b.EvilTrophy; for (unsigned int i = 0; i < sizeof(a.EvilTrophy); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.FakeTrophy; const unsigned char* y = (const unsigned char*)&b.FakeTrophy; for (unsigned int i = 0; i < sizeof(a.FakeTrophy); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.EmoteIconForAvailableWitness; const unsigned char* y = (const unsigned char*)&b.EmoteIconForAvailableWitness; for (unsigned int i = 0; i < sizeof(a.EmoteIconForAvailableWitness); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.ShowTrophyAnimName; const unsigned char* y = (const unsigned char*)&b.ShowTrophyAnimName; for (unsigned int i = 0; i < sizeof(a.ShowTrophyAnimName); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.GameDebugTextScale; const unsigned char* y = (const unsigned char*)&b.GameDebugTextScale; for (unsigned int i = 0; i < sizeof(a.GameDebugTextScale); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.SecsDelayPerGainedWitnessSound; const unsigned char* y = (const unsigned char*)&b.SecsDelayPerGainedWitnessSound; for (unsigned int i = 0; i < sizeof(a.SecsDelayPerGainedWitnessSound); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.SoundStartGame; const unsigned char* y = (const unsigned char*)&b.SoundStartGame; for (unsigned int i = 0; i < sizeof(a.SoundStartGame); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.SoundShowTrophy; const unsigned char* y = (const unsigned char*)&b.SoundShowTrophy; for (unsigned int i = 0; i < sizeof(a.SoundShowTrophy); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.SoundGainedWitness; const unsigned char* y = (const unsigned char*)&b.SoundGainedWitness; for (unsigned int i = 0; i < sizeof(a.SoundGainedWitness); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.SoundOutOfTime; const unsigned char* y = (const unsigned char*)&b.SoundOutOfTime; for (unsigned int i = 0; i < sizeof(a.SoundOutOfTime); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.SoundGotAllWitnesses; const unsigned char* y = (const unsigned char*)&b.SoundGotAllWitnesses; for (unsigned int i = 0; i < sizeof(a.SoundGotAllWitnesses); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.SoundEndNewHighScore; const unsigned char* y = (const unsigned char*)&b.SoundEndNewHighScore; for (unsigned int i = 0; i < sizeof(a.SoundEndNewHighScore); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.SoundEndNoNewScore; const unsigned char* y = (const unsigned char*)&b.SoundEndNoNewScore; for (unsigned int i = 0; i < sizeof(a.SoundEndNoNewScore); ++i) if (x[i] != y[i]) bad = 1; }
    for (int i = 0; i < 0x25; ++i) if (a._body[i] != b._body[i]) bad = 1;
    if (bad) { std::printf("004e0474_TEST FAIL\n"); return 1; }
    std::printf("004e0474_TEST PASS\n");
    return 0;
}