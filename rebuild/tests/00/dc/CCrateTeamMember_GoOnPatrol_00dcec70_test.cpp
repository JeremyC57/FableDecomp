#include <cstdio>
#include <cstring>
#ifdef _MSC_VER
#define FASTCALL __fastcall
#else
#define FASTCALL
#endif
struct CScriptThing { virtual ~CScriptThing() { } void* PImp_Data; void* PImp_Info; CScriptThing() : PImp_Data(0), PImp_Info(0) { } };
struct CCrateTeamManager { long StateCounter[6]; long MemberCount; CCrateTeamManager() : MemberCount(0) { for (int i = 0; i < 6; ++i) StateCounter[i] = 0; } };
struct CQ_OrchardFarmRaidScript { long m_currentTeamID; };
static int g_log[16]; static int g_logCount = 0; static float g_lastDistance = 0.0f; static CScriptThing* g_lastThing = 0;
static void Log(int v) { if (g_logCount < 16) g_log[g_logCount] = v; ++g_logCount; }
struct CScriptInterface {
    virtual ~CScriptInterface() { }
    virtual long FindPatrolPosition(float distance, bool flag) { g_lastDistance = distance; Log(flag ? 11 : 10); return (long)(distance * 100.0f); }
    virtual void SetPatrolTarget(CScriptThing* thing, long position) { g_lastThing = thing; Log(20 + position); }
    virtual void SetThingMoveFlag(CScriptThing* thing, long flag) { g_lastThing = thing; Log(30 + flag); }
};
static bool g_outOfAction = false; static int g_checks = 0;
struct CCrateTeamMember {
    virtual ~CCrateTeamMember() { }
    CScriptInterface*          m_pScript;
    CScriptThing               m_thing;
    CQ_OrchardFarmRaidScript*  ParentClass;
    void*                      PMasterData;
    long                       CurrentAIState;
    long                       PreviousAIState;
    long                       TeamID;
    long                       MemberState;
    CCrateTeamManager*         MyTeam;
    CScriptThing               ThingToPatrolTo;
    bool IsOutOfAction() { ++g_checks; return g_outOfAction; }
    void GoOnPatrol();
};
void CCrateTeamMember::GoOnPatrol()
{
    if (MemberState != 0) {
        IsOutOfAction();
        return;
    }
    if (TeamID == ParentClass->m_currentTeamID) {
        if (IsOutOfAction())
            return;
        m_pScript->SetPatrolTarget(&m_thing, m_pScript->FindPatrolPosition(3.0f, true));
    } else {
        if (IsOutOfAction())
            return;
        m_pScript->SetPatrolTarget(&m_thing, m_pScript->FindPatrolPosition(1.0f, true));
        m_pScript->SetThingMoveFlag(&m_thing, 0);
    }
    --MyTeam->StateCounter[MemberState];
    MemberState = 1;
    ++MyTeam->StateCounter[1];
}
int main()
{
    CScriptInterface script; CCrateTeamManager team; CQ_OrchardFarmRaidScript parent; parent.m_currentTeamID = 2;
    team.StateCounter[0] = 4; team.StateCounter[1] = 1;
    CCrateTeamMember member; member.m_pScript = &script; member.ParentClass = &parent; member.TeamID = 2; member.MemberState = 0; member.MyTeam = &team;
    member.GoOnPatrol();
    const int expect[] = { 11, 320 };
    bool ok = g_logCount == 2 && std::memcmp(g_log, expect, sizeof(expect)) == 0 && g_lastDistance == 3.0f && g_lastThing == &member.m_thing
        && member.MemberState == 1 && team.StateCounter[0] == 3 && team.StateCounter[1] == 2 && g_checks == 1;
    member.GoOnPatrol();
    ok = ok && g_logCount == 2 && g_checks == 2 && member.MemberState == 1 && team.StateCounter[1] == 2;
    member.MemberState = 0; member.TeamID = 3; g_logCount = 0; member.GoOnPatrol();
    const int expect2[] = { 11, 120, 30 };
    ok = ok && g_logCount == 3 && std::memcmp(g_log, expect2, sizeof(expect2)) == 0 && g_lastDistance == 1.0f && member.MemberState == 1
        && team.StateCounter[0] == 2 && team.StateCounter[1] == 3;
    member.MemberState = 0; g_outOfAction = true; g_logCount = 0; member.GoOnPatrol();
    ok = ok && g_logCount == 0 && member.MemberState == 0 && team.StateCounter[0] == 2;
    if (!ok) { std::printf("00dcec70_TEST FAIL\n"); return 1; }
    std::printf("00dcec70_TEST PASS\n");
    return 0;
}