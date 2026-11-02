// Behaviour test for 0x00DCF700 NScript::CQ_OrchardFarmRaidScript::CTeamSpawn::~CTeamSpawn (implicit destructor): members destroyed in
// reverse declaration order (TeamMemberDefName, TeamMemberName, OtherSpawnPoint with its counted-pointer release), then the base.
#include <cstdio>
static int g_order[8];
static int g_n = 0;
static int g_deleted = 0;
class CCPPointerInfo { public: unsigned int RefCount; void (*DeleteFunc)(void*); void* Data; };
static void DeleteThing(void*) { ++g_deleted; }
struct CScriptThingModel {
    void* Data; CCPPointerInfo* Info;
    CScriptThingModel() : Data(0), Info(0) { }
    ~CScriptThingModel()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) { Info->DeleteFunc(Info->Data); delete Info; }
        }
        Data = 0; Info = 0;
        g_order[g_n++] = 3;
    }
};
struct CCharStringModel { int Id; CCharStringModel(int id) : Id(id) { } ~CCharStringModel() { g_order[g_n++] = Id; } };
struct CEntityScriptBaseModel { virtual ~CEntityScriptBaseModel() { g_order[g_n++] = 4; } };
struct CTeamSpawnModel : CEntityScriptBaseModel {
    void* ParentClass; void* PMasterData;
    CScriptThingModel OtherSpawnPoint;
    long TeamID, TeamMemberLimit, TeamRespawnTime;
    CCharStringModel TeamMemberName;
    CCharStringModel TeamMemberDefName;
    long BanditsLeftID;
    CTeamSpawnModel() : TeamMemberName(2), TeamMemberDefName(1) { }
};
int main()
{
    {
        CTeamSpawnModel spawn;
        CCPPointerInfo* info = new CCPPointerInfo;
        info->RefCount = 1; info->DeleteFunc = DeleteThing; info->Data = 0;
        spawn.OtherSpawnPoint.Info = info;
    }
    if (g_n != 4 || g_order[0] != 1 || g_order[1] != 2 || g_order[2] != 3 || g_order[3] != 4 || g_deleted != 1) {
        std::printf("00dcf700_TEST FAIL\n");
        return 1;
    }
    std::printf("00dcf700_TEST PASS\n");
    return 0;
}