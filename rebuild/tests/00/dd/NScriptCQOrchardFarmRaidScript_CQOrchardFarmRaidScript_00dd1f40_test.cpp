// Behaviour test for 0x00DD1F40 NScript::CQ_OrchardFarmRaidScript::~CQ_OrchardFarmRaidScript (implicit): Teams[1], Teams[0]
// (out-of-line ~CCrateTeamManager), TextSystemScriptCode, RemindHeroOfObjectivesTimer, FailReasons[3..0], the CrateList vector
// (virtual element dtors, block freed), CommentTimer, then CScriptBase.
#include <cstdio>
#include <cstddef>
static int g_log[64]; static int g_n = 0;
static void Log(int v) { if (g_n < 64) g_log[g_n++] = v; }
class CTimer { public: long TimerIndex; ~CTimer() { Log(1000 + (int)TimerIndex); } };
class CCharString { public: int Id; ~CCharString() { Log(Id); } };
class CWideString { public: int Id; ~CWideString() { Log(Id); } };
class CScriptThing { public:
    int Id;
    static void* operator new(std::size_t, void* p) { return p; }
    static void operator delete(void*, void*) { }
    static void operator delete(void* p) { ::operator delete(p); }
    virtual ~CScriptThing() { Log(Id); }
};
struct ThingVector {
    CScriptThing* Start; CScriptThing* Finish;
    ~ThingVector() { for (CScriptThing* p = Start; p != Finish; ++p) p->~CScriptThing(); if (Start) { Log(700); ::operator delete(Start); } }
};
struct CCrateTeamManager { int Id; unsigned char Rest[0x3c]; ~CCrateTeamManager() { Log(Id); } };
class CScriptBase { public: virtual ~CScriptBase() { Log(9999); } };
struct CQ_OrchardFarmRaidScript : public CScriptBase {
    void* Game; void* MasterScriptData; long CrateCount; CTimer CommentTimer; bool b0, b1, b2; ThingVector CrateList;
    bool MissionSucceeded; int MissionFailed; CWideString FailReasons[4]; long HeroTeam; long BanditWavesSpawned;
    CTimer RemindHeroOfObjectivesTimer; CCharString TextSystemScriptCode; bool b3, b4, b5, b6; CCrateTeamManager Teams[2];
};
int main()
{
    {
        CQ_OrchardFarmRaidScript s;
        s.CommentTimer.TimerIndex = 1; s.RemindHeroOfObjectivesTimer.TimerIndex = 2; s.TextSystemScriptCode.Id = 50;
        for (int i = 0; i < 4; ++i) s.FailReasons[i].Id = 60 + i;
        s.Teams[0].Id = 70; s.Teams[1].Id = 71;
        s.CrateList.Start = (CScriptThing*)::operator new(sizeof(CScriptThing) * 2);
        for (int j = 0; j < 2; ++j) { new (&s.CrateList.Start[j]) CScriptThing; s.CrateList.Start[j].Id = 80 + j; }
        s.CrateList.Finish = s.CrateList.Start + 2;
    }
    static const int expect[] = { 71, 70, 50, 1002, 63, 62, 61, 60, 80, 81, 700, 1001, 9999 };
    if (g_n != 13) { std::printf("00dd1f40_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 13; ++i) if (g_log[i] != expect[i]) { std::printf("00dd1f40_TEST FAIL\n"); return 1; }
    std::printf("00dd1f40_TEST PASS\n");
    return 0;
}