#include <cstdio>
#include <new>
struct CThingManager_Calls { virtual long CreateTimer() = 0; };
CThingManager_Calls* g_pThingManager_0143e8f8;
class CBaseClass {
public:  virtual void v00() { } CBaseClass() { } };
struct CCountedPointer_Retail { void* Data; void* Info; CCountedPointer_Retail() : Data(0), Info(0) { } };
struct CScriptThing_Retail : CBaseClass {
    virtual void v00() { }
    CCountedPointer_Retail PImp;
    CScriptThing_Retail() : CBaseClass(), PImp() { }
};
class CTimer { public: long TimerIndex; CTimer() : TimerIndex(g_pThingManager_0143e8f8->CreateTimer()) { } };
struct CCrateTeamManager {
    long               StateCounter[6];
    long               MemberCount;
    CTimer             TeamReinforcementsTimer;
    long               MaxTeamMembers;
    CScriptThing_Retail CrateDropPos;
    CScriptThing_Retail TeamCrateCarrier;
    CCrateTeamManager* EnemyTeam;
    CCrateTeamManager();
};
CCrateTeamManager::CCrateTeamManager()
    : TeamReinforcementsTimer(), CrateDropPos(), TeamCrateCarrier()
{
    for (long i = 0; i < 6; ++i)
        StateCounter[i] = 0;
    MemberCount = 0;
}
struct CTestManager : CThingManager_Calls { virtual long CreateTimer() { return 77; } };
int main()
{
    CTestManager m; g_pThingManager_0143e8f8 = &m;
    unsigned char raw[sizeof(CCrateTeamManager)];
    for (unsigned i = 0; i < sizeof(raw); ++i) raw[i] = 0xee;
    CCrateTeamManager* t = new (raw) CCrateTeamManager();
    for (long i = 0; i < 6; ++i) if (t->StateCounter[i] != 0) { std::printf("00cdb360_TEST FAIL\n"); return 1; }
    if (t->MemberCount != 0 || t->TeamReinforcementsTimer.TimerIndex != 77 || t->CrateDropPos.PImp.Data || t->CrateDropPos.PImp.Info
        || t->TeamCrateCarrier.PImp.Data || t->TeamCrateCarrier.PImp.Info || sizeof(CCrateTeamManager) != 0x40) { std::printf("00cdb360_TEST FAIL\n"); return 1; }
    std::printf("00cdb360_TEST PASS\n");
    return 0;
}