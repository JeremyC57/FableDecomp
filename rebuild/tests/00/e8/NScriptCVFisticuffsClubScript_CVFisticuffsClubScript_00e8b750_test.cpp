// Behaviour test for NScript::CV_FisticuffsClubScript::~CV_FisticuffsClubScript (0x00E8B750, compiler-generated): RegionNames[4..0], Village, SFighterTwo, SFighterOne, Fighter2Timer, Fighter1Timer, then ~CScriptBase.
#include <cstdio>
#include <cstring>
#include <cstdlib>
static char g_log[512];
static void Log(char c) { std::size_t n = std::strlen(g_log); g_log[n] = c; g_log[n + 1] = 0; }
struct InfoModel { unsigned long RefCount; void (*DeleteFunc)(void*); void* Data; };
static void DeleteData(void* data) { Log(*(const char*)data); }
struct ScriptThingModel {                        // CScriptThing: PImp counted pointer released in the dtor
    void* Data; InfoModel* Info;
    ScriptThingModel() : Data(0), Info(0) { }
    void Set(const char* tag) { Info = new InfoModel; Info->RefCount = 1; Info->DeleteFunc = DeleteData; Info->Data = (void*)tag; Data = (void*)tag; }
    ~ScriptThingModel()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) { Info->DeleteFunc(Info->Data); delete Info; }
        }
        Data = 0; Info = 0;
    }
};
static long g_lastDeregistered = -1;
struct TimerModel { long TimerIndex; char Tag; ~TimerModel() { g_lastDeregistered = TimerIndex; Log(Tag); } };
struct StringModel { char Tag; StringModel() : Tag('?') { } ~StringModel() { Log(Tag); } };
struct ScriptBaseModel { virtual ~ScriptBaseModel() { Log('S'); } };
struct FisticuffsScriptModel : ScriptBaseModel {
    TimerModel       Fighter1Timer;
    TimerModel       Fighter2Timer;
    ScriptThingModel SFighterOne;
    ScriptThingModel SFighterTwo;
    ScriptThingModel Village;
    StringModel      RegionNames[5];
};
int main()
{
    FisticuffsScriptModel* s = new FisticuffsScriptModel;
    s->Fighter1Timer.TimerIndex = 11; s->Fighter1Timer.Tag = 'A';
    s->Fighter2Timer.TimerIndex = 12; s->Fighter2Timer.Tag = 'B';
    s->SFighterOne.Set("1"); s->SFighterTwo.Set("2");
    InfoModel shared = { 2, DeleteData, (void*)"v" };
    s->Village.Info = &shared;                     // still referenced elsewhere: released, not deleted
    for (int i = 0; i < 5; ++i) s->RegionNames[i].Tag = (char)('0' + i);
    ScriptBaseModel* p = s;
    delete p;
    if (std::strcmp(g_log, "4321021BAS") != 0 || shared.RefCount != 1 || g_lastDeregistered != 11) { std::printf("00e8b750_TEST FAIL\n"); return 1; }
    std::printf("00e8b750_TEST PASS\n");
    return 0;
}