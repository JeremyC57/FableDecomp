// Behaviour test for NScript::CQ_OrchardFarmRaidScript::CCrateTeamMember::Main (0x00DCE230): before the introduction is done a
// member is removed (and taken off its team's member count and state counter) if the hero came in the wrong way; a bandit on the
// hero's team says FETCHING_03 after any cutscene; until Whisper spawns it patrols, comments on AI-state changes (DEFENDING /
// FOLLOWING / ATTACKING) and walks the member-state machine (1 idle -> 2 fetch a free nearby crate, -> 3 defend the team carrier,
// -> 4 chase the enemy carrier, 2 -> 5 carrying, back to 0); then it walks to the team exit marker and fades out.
// A log model of the state machine (one decision per frame).
#include <cstdio>
#include <cstring>
static char g_log[4096];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct Team { long StateCounter[6]; long MemberCount; bool CarrierAlive, CarrierIsMe, CarrierCarrying; };
struct Member {
    Team* MyTeam; Team* Enemy; long MemberState, TeamID, HeroTeam;
    bool Carrying, CrateFree;
    void SetState(long s) { --MyTeam->StateCounter[MemberState]; MemberState = s; ++MyTeam->StateCounter[s]; }
    void Step()
    {
        if (MemberState == 1 && !MyTeam->CarrierAlive && MyTeam->StateCounter[2] == 0 && CrateFree) { SetState(2); Log("FETCHING"); }
        if (MemberState == 1 && MyTeam->CarrierAlive && !MyTeam->CarrierIsMe) { SetState(3); Log("DEFENDING"); }
        if (MemberState == 3 && (!MyTeam->CarrierAlive || !MyTeam->CarrierCarrying)) { SetState(0); Log("stop"); }
        if (MemberState == 1 && Enemy->CarrierAlive) { SetState(4); Log("FETCHING"); }
        if (MemberState == 4 && (!Enemy->CarrierAlive || !Enemy->CarrierCarrying)) SetState(0);
        if (MemberState == 2 && Carrying) { SetState(5); MyTeam->CarrierIsMe = true; MyTeam->CarrierAlive = true; Log("REQUEST_PROTECTION"); }
        if (MemberState == 5 && !Carrying) { MyTeam->CarrierAlive = false; SetState(0); }
    }
};
int main()
{
    Team mine = { { 0, 1, 0, 0, 0, 0 }, 1, false, false, false };
    Team enemy = { { 0 }, 1, false, false, false };
    Member m = { &mine, &enemy, 1, 1, 1, false, true };
    g_log[0] = 0;
    m.Step();                 // idle, no carrier, free crate: fetch
    m.Carrying = true; m.Step();   // picked the crate up: carrier
    bool ok = m.MemberState == 5 && mine.StateCounter[5] == 1 && mine.StateCounter[1] == 0 && mine.CarrierIsMe;
    m.Carrying = false; m.Step();  // dropped it: back to 0, carrier cleared
    ok = ok && m.MemberState == 0 && !mine.CarrierAlive && mine.StateCounter[0] == 1;
    Member d = { &mine, &enemy, 1, 0, 1, false, false };
    ++mine.StateCounter[1];
    mine.CarrierAlive = true; mine.CarrierIsMe = false; mine.CarrierCarrying = true;
    d.Step();                 // defend the carrier
    ok = ok && d.MemberState == 3;
    mine.CarrierCarrying = false; d.Step();   // carrier lost the crate
    ok = ok && d.MemberState == 0 && !std::strcmp(g_log, "FETCHING;REQUEST_PROTECTION;DEFENDING;stop;");
    if (!ok) std::printf("%s\n", g_log);
    std::printf(ok ? "00dce230_TEST PASS\n" : "00dce230_TEST FAIL\n");
    return ok ? 0 : 1;
}
