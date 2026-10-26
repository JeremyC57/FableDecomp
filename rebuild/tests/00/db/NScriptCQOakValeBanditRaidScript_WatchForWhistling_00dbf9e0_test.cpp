#include <cstdio>
#include <cstring>
// Behaviour test for CQ_OakValeBanditRaidScript::WatchForWhistling (0x00DBF9E0): an opening whistle, then frame by
// frame (region loaded, whistling not stopped) a new random whistle whenever the last sound finished and a bandit lives.
static int g_seq = 0;
static int test_rand() { return g_seq++; }
class CScriptThing { public: bool Alive; bool IsAlive() const { return Alive; } };
struct CGSI {
    int Frames; int Plays; int LastSound; bool BanditAlive; int RegionFrom; CScriptThing Hero; bool* Stop;
    char Last[32];
    void NewScriptFrame() { ++Frames; if (Frames == 8) *Stop = false; }
    bool IsRegionLoaded(const char*) { return Frames >= RegionFrom; }
    CScriptThing& GetHero() { return Hero; }
    CScriptThing GetNearestWithScriptName(const CScriptThing&, const char* name)
    { CScriptThing t; t.Alive = BanditAlive && std::strcmp(name, "OakValeBanditRaidBandit") == 0; return t; }
    unsigned long PlaySoundOnThing(const CScriptThing&, const char* sound)
    { ++Plays; std::strcpy(Last, sound); LastSound = Frames; return (unsigned long)Plays; }
    bool IsSoundPlaying(unsigned long) { return Frames - LastSound < 2; }
};
struct CQ_OakValeBanditRaidScript {
    CGSI* GSI; unsigned long SoundPlayed; bool StopWhistling;
    bool IsActiveThreadTerminating() { return GSI->Frames > 20; }
    void WatchForWhistling()
    {
        SoundPlayed = GSI->PlaySoundOnThing(GSI->GetNearestWithScriptName(GSI->GetHero(), "OakValeBanditRaidBandit"), "SND_BANDIT_WHISTLE_01");
        if (IsActiveThreadTerminating())
            return;
        for (;;) {
            if (!GSI->IsRegionLoaded("BanditCampPathEntrance")) {
                if (IsActiveThreadTerminating())
                    return;
                while (!GSI->IsRegionLoaded("BanditCampPathEntrance")) {
                    GSI->NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return;
                }
                if (IsActiveThreadTerminating())
                    return;
            }
            if (StopWhistling == true) {
                if (IsActiveThreadTerminating())
                    return;
                while (StopWhistling) {
                    GSI->NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return;
                }
                if (IsActiveThreadTerminating())
                    return;
            }
            if (!GSI->IsSoundPlaying(SoundPlayed)) {
                if (IsActiveThreadTerminating())
                    return;
                if (GSI->GetNearestWithScriptName(GSI->GetHero(), "OakValeBanditRaidBandit").IsAlive()) {
                    if (IsActiveThreadTerminating())
                        return;
                    switch (test_rand() % 3) {
                    case 0: SoundPlayed = GSI->PlaySoundOnThing(GSI->GetNearestWithScriptName(GSI->GetHero(), "OakValeBanditRaidBandit"), "SND_BANDIT_WHISTLE_01"); break;
                    case 1: SoundPlayed = GSI->PlaySoundOnThing(GSI->GetNearestWithScriptName(GSI->GetHero(), "OakValeBanditRaidBandit"), "SND_BANDIT_WHISTLE_02"); break;
                    case 2: SoundPlayed = GSI->PlaySoundOnThing(GSI->GetNearestWithScriptName(GSI->GetHero(), "OakValeBanditRaidBandit"), "SND_BANDIT_WHISTLE_03"); break;
                    }
                }
            }
            GSI->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
    }
};
int main()
{
    CGSI gsi; gsi.Frames = 0; gsi.Plays = 0; gsi.LastSound = 0; gsi.BanditAlive = true; gsi.RegionFrom = 3;
    CQ_OakValeBanditRaidScript s; s.GSI = &gsi; s.SoundPlayed = 0; s.StopWhistling = true; gsi.Stop = &s.StopWhistling;
    s.WatchForWhistling();
    // opening whistle at frame 0; waits for the region (frame 3) and the stop flag (cleared at frame 8);
    // then a whistle every 2 frames from frame 8: 8,10,...,20 -> 7 more, the 7th is rand()==6 -> WHISTLE_01
    bool ok = gsi.Frames == 21 && gsi.Plays == 8 && s.SoundPlayed == 8 && std::strcmp(gsi.Last, "SND_BANDIT_WHISTLE_01") == 0;
    g_seq = 1; gsi.Frames = 0; gsi.Plays = 0; gsi.LastSound = 0; gsi.BanditAlive = false; s.StopWhistling = false;
    s.WatchForWhistling();
    ok = ok && gsi.Plays == 1 && s.SoundPlayed == 1;
    if (!ok) { std::printf("00dbf9e0_TEST FAIL %d %d %lu %s\n", gsi.Frames, gsi.Plays, s.SoundPlayed, gsi.Last); return 1; }
    std::printf("00dbf9e0_TEST PASS\n");
    return 0;
}