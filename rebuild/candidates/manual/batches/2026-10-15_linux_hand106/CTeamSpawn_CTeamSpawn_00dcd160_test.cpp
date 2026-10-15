#include <cstdio>
#include <cstring>
static char g_seq[16] = "";
struct CBase { virtual ~CBase() { } CBase() { std::strcat(g_seq, "b"); } };
struct CCountedPointer_Imp { void* m_pObject; void* m_pCount; CCountedPointer_Imp() : m_pObject(0), m_pCount(0) { } };
struct CScriptThing : CBase {
    CCountedPointer_Imp PImp;
    CScriptThing() { }
    CScriptThing(const CScriptThing& o) { PImp = o.PImp; std::strcat(g_seq, "c"); }
};
struct CCharString { const char* m_pData; CCharString() : m_pData("") { std::strcat(g_seq, "s"); } };
struct CGameScriptInterface { int id; };
struct CQ_OrchardFarmRaidScript { unsigned char _pad[0x40]; CGameScriptInterface* GSI; };
struct CQ_SunnyvaleMasterData { int id; };
struct CEntityScriptRoot { CGameScriptInterface* PQuest; CEntityScriptRoot(CGameScriptInterface* q) : PQuest(q) { } virtual ~CEntityScriptRoot() { } };
struct CEntityScriptBase : CEntityScriptRoot {
    CScriptThing Thing;
    CEntityScriptBase(CQ_OrchardFarmRaidScript* parent, const CScriptThing& thing) : CEntityScriptRoot(parent->GSI), Thing(thing) { }
};
struct CTeamSpawn : CEntityScriptBase {
    CQ_OrchardFarmRaidScript* ParentClass; CQ_SunnyvaleMasterData* PMasterData; CScriptThing OtherSpawnPoint;
    long TeamID, TeamMemberLimit, TeamRespawnTime; CCharString TeamMemberName, TeamMemberDefName; long BanditsLeftID;
    CTeamSpawn(CQ_OrchardFarmRaidScript* parent, CQ_SunnyvaleMasterData* masterData, const CScriptThing& thing)
        : CEntityScriptBase(parent, thing), ParentClass(parent) { PMasterData = masterData; }
};
int main()
{
    CGameScriptInterface gsi; CQ_OrchardFarmRaidScript parent; parent.GSI = &gsi; CQ_SunnyvaleMasterData md;
    int obj = 0;
    CScriptThing thing; thing.PImp.m_pObject = &obj;
    g_seq[0] = 0;
    CTeamSpawn t(&parent, &md, thing);
    bool ok = t.PQuest == &gsi && t.ParentClass == &parent && t.PMasterData == &md && t.Thing.PImp.m_pObject == &obj &&
              t.OtherSpawnPoint.PImp.m_pObject == 0 && t.OtherSpawnPoint.PImp.m_pCount == 0 && !std::strcmp(g_seq, "bcbss");
    if (!ok) { std::printf("00dcd160_TEST FAIL\n"); return 1; }
    std::printf("00dcd160_TEST PASS\n");
    return 0;
}
