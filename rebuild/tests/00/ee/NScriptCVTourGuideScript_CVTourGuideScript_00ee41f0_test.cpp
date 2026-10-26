#include <cstdio>
// Behaviour test for 00ee41f0: CV_TourGuideScript's ctor keeps the interface/data pair and default-constructs 18 waypoint
// records (3 strings each), three 5-string response tables and an empty NextTourWaypoint handle.
static int g_strings = 0;
class CCharString { public: const char* p; CCharString() : p("") { ++g_strings; } };
class CScriptThing { public: void* Data; void* Info; CScriptThing() : Data(0), Info(0) {} };
struct Interface { int unused; };
struct Base { Interface* Game; void* MasterScriptData; Base(void* data, Interface& game) : Game(&game), MasterScriptData(data) {} };
struct WaypointData { CCharString locMarker, locTextOverheard, locTextRequested; WaypointData() {} };
struct Script : Base {
    WaypointData WaypointInfo[18]; bool flags[7];
    CCharString RandomGuideResponse[5], RandomFollowerResponseM[5], RandomFollowerResponseF[5];
    long WaypointCounter; CScriptThing NextTourWaypoint; bool spoken, overheard;
    Script(void* data, Interface& game) : Base(data, game) {}
};
int main()
{
    Interface gi; int data = 0;
    Script* s = new Script(&data, gi);
    int ok = s->Game == &gi && s->MasterScriptData == &data && g_strings == 18 * 3 + 15
          && s->WaypointInfo[17].locTextRequested.p[0] == 0 && s->NextTourWaypoint.Data == 0 && s->NextTourWaypoint.Info == 0;
    delete s;
    if (!ok) { std::printf("00ee41f0_TEST FAIL\n"); return 1; }
    std::printf("00ee41f0_TEST PASS\n");
    return 0;
}