// Behaviour test for NScript::CQ_GuildTrainingScript::CApprenticeSpeedTest::Main (0x00D3E2E0): the race state machine (0 boast and
// challenge, 1 race running, 2 failed / rematch, 3 won) and the post-race banter line choice (lines 1-5 scripted, then the apprentice
// on even lines and the friend on odd lines). A model of the transitions and the line pick.
#include <cstdio>
#include <cstring>
// talk to the apprentice in a race mode; returns the new mode
static int Talk(int mode, bool acceptChallenge, bool reachedPlatform, long raceTime)
{
    switch (mode) {
    case 0: case 2: return acceptChallenge ? 1 : mode;
    case 1:
        if (!reachedPlatform) return 2;      // NO_PLATFORM
        if (raceTime <= 0) return 2;         // TOO_SLOW
        return 3;                            // SUCCEED
    }
    return mode;
}
static int TotalTimerExpired(int mode) { return mode == 1 ? 2 : mode; }
static const char* PostLine(long line)
{
    if (line < 6) {
        switch (line) {
        case 1: return "FRIEND_FIRST";
        case 2: return "APPRENTICE_SECOND";
        case 3: return "FRIEND_THIRD";
        case 4: return "APPRENTICE_FOURTH";
        case 5: return "FRIEND_FIFTH";
        }
        return "";
    }
    return line % 2 == 0 ? "APPRENTICE_SIXTH" : "FRIEND_SEVENTH";
}
int main()
{
    bool ok = Talk(0, false, false, 0) == 0 && Talk(0, true, false, 0) == 1 && Talk(1, false, false, 9) == 2 && Talk(1, false, true, 0) == 2
        && Talk(1, false, true, 5) == 3 && Talk(2, true, false, 0) == 1 && Talk(2, false, false, 0) == 2 && TotalTimerExpired(1) == 2 && TotalTimerExpired(3) == 3;
    ok = ok && !std::strcmp(PostLine(0), "") && !std::strcmp(PostLine(1), "FRIEND_FIRST") && !std::strcmp(PostLine(4), "APPRENTICE_FOURTH")
        && !std::strcmp(PostLine(6), "APPRENTICE_SIXTH") && !std::strcmp(PostLine(7), "FRIEND_SEVENTH");
    std::printf(ok ? "00d3e2e0_TEST PASS\n" : "00d3e2e0_TEST FAIL\n");
    return ok ? 0 : 1;
}
