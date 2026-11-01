// Behaviour test for NScript::CV_TourGuideScript::~CV_TourGuideScript (0x00EE6C50, compiler-generated): NextTourWaypoint, the three CCharString[5] arrays (each in reverse), WaypointInfo[17..0] (requested, overheard, marker), then ~CScriptBase.
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
struct WaypointDataModel { StringModel locMarker; StringModel locTextOverheard; StringModel locTextRequested; };
struct TourGuideScriptModel : ScriptBaseModel {
    WaypointDataModel WaypointInfo[18];
    StringModel       RandomGuideResponse[5];
    StringModel       RandomFollowerResponseM[5];
    StringModel       RandomFollowerResponseF[5];
    long              WaypointCounter;
    ScriptThingModel  NextTourWaypoint;
};
int main()
{
    TourGuideScriptModel* s = new TourGuideScriptModel;
    for (int i = 0; i < 18; ++i) { s->WaypointInfo[i].locMarker.Tag = 'm'; s->WaypointInfo[i].locTextOverheard.Tag = 'o'; s->WaypointInfo[i].locTextRequested.Tag = 'r'; }
    for (int i = 0; i < 5; ++i) { s->RandomGuideResponse[i].Tag = (char)('a' + i); s->RandomFollowerResponseM[i].Tag = (char)('f' + i); s->RandomFollowerResponseF[i].Tag = (char)('0' + i); }
    s->NextTourWaypoint.Set("p");
    ScriptBaseModel* p = s;
    delete p;
    char expect[96] = "p43210jihgfedcba";
    for (int i = 0; i < 18; ++i) std::strcat(expect, "rom");
    std::strcat(expect, "S");
    if (std::strcmp(g_log, expect) != 0) { std::printf("00ee6c50_TEST FAIL\n"); return 1; }
    std::printf("00ee6c50_TEST PASS\n");
    return 0;
}