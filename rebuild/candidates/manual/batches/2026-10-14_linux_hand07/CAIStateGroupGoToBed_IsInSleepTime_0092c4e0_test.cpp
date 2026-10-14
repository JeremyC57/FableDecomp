#include <cstdio>
// Behaviour model of CAIStateGroup_GoToBed::IsInSleepTime @ 0x0092C4E0.
static long g_start = 0, g_end = 0;
struct CGameTime { bool IsTimeOfDayInRange(long start, long end) { g_start = start; g_end = end; return start == 1300 + 10 || end == 1400 + 10; } };
struct CWorld { unsigned char _pad[0x1c]; CGameTime* m_pTime; };
struct CThingAICreature { void GetSleepTimes(long* pStart, long* pEnd) { *pStart = 1300; *pEnd = 1400; } };
struct CAIBrain { unsigned char _pad[0x18]; CThingAICreature* PCreature; };
static CWorld g_world; static CGameTime g_time;
struct CAIStateGroupBase { virtual void v00() {} CAIBrain* PBrain; CWorld* GetPWorld() { return &g_world; } };
struct CAIStateGroup_GoToBed : CAIStateGroupBase { unsigned char PBed[8]; long m_sleepTimeOffset, m_bedTimeDelay, m_wakeTimeDelay, m_pad28, m_state;
    bool IsInSleepTime() { CGameTime* pTime = GetPWorld()->m_pTime; long sleepEnd; long sleepStart; PBrain->PCreature->GetSleepTimes(&sleepStart, &sleepEnd); return pTime->IsTimeOfDayInRange(sleepStart + m_bedTimeDelay, sleepEnd); } };
int main()
{
    g_world.m_pTime = &g_time; CAIBrain brain; CThingAICreature creature; brain.PCreature = &creature;
    CAIStateGroup_GoToBed g; g.PBrain = &brain; g.m_bedTimeDelay = 10; g.m_wakeTimeDelay = 10;
    bool r = g.IsInSleepTime();
    bool ok = r == true && g_start == 1310 && g_end == 1400;
    g.m_bedTimeDelay = 3; g.m_wakeTimeDelay = 3;
    ok = ok && g.IsInSleepTime() == false;
    if (!ok) { std::printf("0092c4e0_TEST FAIL\n"); return 1; }
    std::printf("0092c4e0_TEST PASS\n");
    return 0;
}
