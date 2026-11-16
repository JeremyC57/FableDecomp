// Behaviour model of CCrateTeamMember::SetState 0x00DCEC50: the team's head count for the old state drops by one, the member takes the new
// state, and the count for the new state rises by one (setting the same state leaves the counts unchanged).
#include <cstdio>
struct Team { long StateCounter[6]; };
struct Member {
    long MemberState; Team* MyTeam;
    void SetState(long state) { MyTeam->StateCounter[MemberState]--; MemberState = state; MyTeam->StateCounter[state]++; }
};
int main()
{
    Team team = { { 3, 1, 0, 2, 0, 0 } };
    Member m = { 0, &team };
    m.SetState(2);
    if (m.MemberState != 2 || team.StateCounter[0] != 2 || team.StateCounter[2] != 1) { std::printf("FAIL 1\n"); return 1; }
    m.SetState(2);
    if (m.MemberState != 2 || team.StateCounter[2] != 1) { std::printf("FAIL same\n"); return 1; }
    m.SetState(5);
    if (team.StateCounter[2] != 0 || team.StateCounter[5] != 1 || team.StateCounter[0] != 2 || team.StateCounter[1] != 1) { std::printf("FAIL 2\n"); return 1; }
    long total = 0; for (int i = 0; i < 6; ++i) total += team.StateCounter[i];
    if (total != 6) { std::printf("FAIL total\n"); return 1; }
    std::printf("PARITY_OK_dcec50\n");
    return 0;
}
