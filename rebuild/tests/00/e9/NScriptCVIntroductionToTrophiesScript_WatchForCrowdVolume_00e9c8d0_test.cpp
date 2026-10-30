// Behaviour test for NScript::CV_IntroductionToTrophiesScript::WatchForCrowdVolume (0x00E9C8D0): while the crowd opinion
// has reached the required level, opinion 0 / 1 arm the opinion timer (once while inside the tavern, every frame outside)
// and raise the requirement to 1 / 2 when the armed timer runs out; opinion 2 waits for the region-loaded message and
// resets the requirement to 0. The thread ends when it is terminated.
#include <cstdio>

struct Game {
    int frames, regionLoadedAt;
    long timer;
    void NewScriptFrame() { ++frames; if (timer > 0) --timer; }
    bool MsgOnRegionLoaded(const char*) { return frames >= regionLoadedAt; }
};

class CTimer { public:
    Game* G;
    CTimer& operator=(long t) { G->timer = t; return *this; }
    long GetTime() const { return G->timer; }
};

static const long TrophiesInsideTavernTimer = 2;

struct CV_IntroductionToTrophiesScript {
    Game* Game_;
    int OpinionCurrent, OpinionRequired;
    bool InsideTavern, OpinionTimerReset;
    CTimer OpinionTimer;
    int termAt, checks, arms;
    bool IsActiveThreadTerminating() { ++checks; return checks >= termAt; }
    void Arm() { OpinionTimer = TrophiesInsideTavernTimer; ++arms; }
    void WatchForCrowdVolume()
    {
        const char* region = "";
        while (!IsActiveThreadTerminating()) {
            if (OpinionCurrent == OpinionRequired) {
                if (IsActiveThreadTerminating()) return;
                if (OpinionCurrent == 0 || OpinionCurrent == 1) {
                    if (IsActiveThreadTerminating()) return;
                    if (InsideTavern) {
                        if (IsActiveThreadTerminating()) return;
                        if (!OpinionTimerReset) {
                            if (IsActiveThreadTerminating()) return;
                            Arm();
                            OpinionTimerReset = true;
                        }
                    } else {
                        if (IsActiveThreadTerminating()) return;
                        Arm();
                    }
                    if (OpinionTimer.GetTime() == 0 && OpinionTimerReset) {
                        if (IsActiveThreadTerminating()) return;
                        OpinionRequired = OpinionCurrent + 1;
                        OpinionTimerReset = false;
                    }
                } else if (OpinionCurrent == 2) {
                    if (IsActiveThreadTerminating()) return;
                    while (!Game_->MsgOnRegionLoaded(region)) {
                        Game_->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return;
                    }
                    if (IsActiveThreadTerminating()) return;
                    OpinionRequired = 0;
                }
            }
            Game_->NewScriptFrame();
        }
    }
};

int main()
{
    Game g; g.frames = 0; g.regionLoadedAt = 0; g.timer = 0;
    CV_IntroductionToTrophiesScript s;
    s.Game_ = &g; s.OpinionTimer.G = &g; s.checks = 0; s.arms = 0;
    // inside the tavern at opinion 0: the timer is armed once, then the requirement rises to 1 when it runs out
    s.OpinionCurrent = 0; s.OpinionRequired = 0; s.InsideTavern = true; s.OpinionTimerReset = false; s.termAt = 40;
    s.WatchForCrowdVolume();
    if (s.OpinionRequired != 1 || s.arms != 1 || s.OpinionTimerReset) { std::printf("00e9c8d0_TEST FAIL\n"); return 1; }
    // outside the tavern the timer is re-armed every pass and never armed-and-reset, so the requirement stays
    s.checks = 0; s.arms = 0; s.OpinionCurrent = 1; s.OpinionRequired = 1; s.InsideTavern = false; s.termAt = 20;
    s.WatchForCrowdVolume();
    if (s.OpinionRequired != 1 || s.arms < 2) { std::printf("00e9c8d0_TEST FAIL\n"); return 1; }
    // opinion 2 waits for the region-loaded message, then resets the requirement
    s.checks = 0; s.OpinionCurrent = 2; s.OpinionRequired = 2; g.frames = 0; g.regionLoadedAt = 3; s.termAt = 12;
    s.WatchForCrowdVolume();
    if (s.OpinionRequired != 0 || g.frames < 3) { std::printf("00e9c8d0_TEST FAIL\n"); return 1; }
    std::printf("00e9c8d0_TEST PASS\n");
    return 0;
}