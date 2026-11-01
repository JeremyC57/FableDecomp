// Behaviour test for NScript::CV_SwordInTheStoneScript::~CV_SwordInTheStoneScript (0x00ED7640, compiler-generated): the crowd
// sword timer is deregistered with the script interface first, then CrowdMemberPullingNull and CrowdMemberPullingCurrent drop
// their implementation counted pointers (the info block's delete function and the block itself on the last reference, both
// words zeroed), then ~CScriptBase.
#include <cstdio>
#include <cstring>
static char g_log[32];
static long g_deregistered = -1;
static int g_deleteFuncCalls;
static void Log(const char* s) { std::strcat(g_log, s); }
struct InfoModel { unsigned long RefCount; void (*DeleteFunc)(void*); void* Data; };
static void DeleteData(void* data) { ++g_deleteFuncCalls; Log((const char*)data); }
struct ScriptThingModel {
    void* Data; InfoModel* Info;
    ~ScriptThingModel()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) { Info->DeleteFunc(Info->Data); delete Info; }
        }
        Data = 0; Info = 0;
    }
};
struct TimerModel { long TimerIndex; ~TimerModel() { g_deregistered = TimerIndex; Log("T"); } };
struct ScriptBaseModel { virtual ~ScriptBaseModel() { Log("S"); } };
struct SwordScriptModel : ScriptBaseModel {
    ScriptThingModel CrowdMemberPullingCurrent;
    ScriptThingModel CrowdMemberPullingNull;
    TimerModel CrowdSwordTimer;
};
int main()
{
    InfoModel* current = new InfoModel; current->RefCount = 1; current->DeleteFunc = DeleteData; current->Data = (void*)"c";
    InfoModel shared = { 2, DeleteData, (void*)"n" };
    ScriptBaseModel* p = new SwordScriptModel;
    SwordScriptModel* s = (SwordScriptModel*)p;
    s->CrowdMemberPullingCurrent.Data = current->Data; s->CrowdMemberPullingCurrent.Info = current;
    s->CrowdMemberPullingNull.Data = shared.Data; s->CrowdMemberPullingNull.Info = &shared;
    s->CrowdSwordTimer.TimerIndex = 42;
    delete p;
    if (std::strcmp(g_log, "TcS") != 0 || g_deregistered != 42 || g_deleteFuncCalls != 1 || shared.RefCount != 1) {
        std::printf("00ed7640_TEST FAIL\n"); return 1;
    }
    std::printf("00ed7640_TEST PASS\n");
    return 0;
}