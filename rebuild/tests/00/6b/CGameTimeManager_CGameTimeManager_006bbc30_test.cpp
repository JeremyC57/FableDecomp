// Behaviour test for CGameTimeManager::CGameTimeManager (0x006BBC30): member defaults, an empty stop-time set whose
// header points at itself, Time = PrevTime = VillageTime = start hour / 24, TimeStep = 1 / day speed, and two console
// commands (SetTimeOfDay, SetDaySpeed) bound to the manager, the console being created on first use only.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>

struct CConsoleCommand { const char* Name; void* Object; int Type; };
struct CConsole { CConsoleCommand* Inputs[4]; int Count; CConsole() : Count(0) {} void AddInput(CConsoleCommand* c) { Inputs[Count++] = c; } };
static CConsole* g_pConsole = 0; static int g_consoles = 0;
inline CConsole* GetConsole() { if (g_pConsole == 0) { g_pConsole = new CConsole(); ++g_consoles; } return g_pConsole; }
struct SSetNode { char Color; SSetNode* Parent; SSetNode* Left; SSetNode* Right; long Value; };
struct CLongSet {
    SSetNode* _M_header; unsigned long _M_node_count;
    CLongSet() : _M_header(0) {
        _M_header = (SSetNode*)std::malloc(sizeof(SSetNode)); _M_node_count = 0;
        _M_header->Color = 0; _M_header->Parent = 0; _M_header->Left = _M_header; _M_header->Right = _M_header;
    }
};
struct CEnvironmentDef { float StartTime, DaySpeed; };
struct CEnvironment { const CEnvironmentDef* Def; };
struct CWorld { CEnvironment* Environment; };
struct CMainGameComponent { int x; };
struct CGameTimeManager {
    CWorld* World; CMainGameComponent* GameComponent; float Time, PrevTime, TimeStep; long DayCount; bool UsingVillageTime;
    float VillageTime; bool VillageDay, FastForwardingToTime; float TimeFastForwardingTo, SecondsLeftToFastForwardTime;
    CLongSet StopTimeIndexes; long NextStopTimeIndex;
    CGameTimeManager(CMainGameComponent& gameComponent, CWorld& world)
        : World(&world), GameComponent(&gameComponent), DayCount(0), UsingVillageTime(false), VillageDay(true),
          FastForwardingToTime(false), TimeFastForwardingTo(-1.0f), SecondsLeftToFastForwardTime(0), NextStopTimeIndex(1)
    {
        const CEnvironmentDef* def = World->Environment->Def;
        Time = def->StartTime * (1.0f / 24.0f);
        PrevTime = Time;
        VillageTime = Time;
        TimeStep = 1.0f / def->DaySpeed;
        CConsoleCommand* a = new CConsoleCommand; a->Name = "SetTimeOfDay"; a->Object = this; a->Type = 3;
        GetConsole()->AddInput(a);
        CConsoleCommand* b = new CConsoleCommand; b->Name = "SetDaySpeed"; b->Object = this; b->Type = 3;
        GetConsole()->AddInput(b);
    }
};

int main()
{
    int ok = 1;
    CEnvironmentDef def; def.StartTime = 6.0f; def.DaySpeed = 4.0f;
    CEnvironment env; env.Def = &def; CWorld world; world.Environment = &env; CMainGameComponent gc;
    CGameTimeManager m(gc, world);
    if (m.World != &world || m.GameComponent != &gc || m.DayCount != 0 || m.UsingVillageTime || !m.VillageDay
        || m.FastForwardingToTime || m.TimeFastForwardingTo != -1.0f || m.SecondsLeftToFastForwardTime != 0 || m.NextStopTimeIndex != 1) ok = 0;
    if (std::fabs(m.Time - 0.25f) > 1e-6f || m.PrevTime != m.Time || m.VillageTime != m.Time || std::fabs(m.TimeStep - 0.25f) > 1e-6f) ok = 0;
    if (m.StopTimeIndexes._M_node_count != 0 || m.StopTimeIndexes._M_header->Left != m.StopTimeIndexes._M_header
        || m.StopTimeIndexes._M_header->Parent != 0) ok = 0;
    if (g_consoles != 1 || g_pConsole->Count != 2 || std::strcmp(g_pConsole->Inputs[0]->Name, "SetTimeOfDay") != 0
        || std::strcmp(g_pConsole->Inputs[1]->Name, "SetDaySpeed") != 0 || g_pConsole->Inputs[1]->Object != &m) ok = 0;
    CGameTimeManager m2(gc, world);   // the console is reused
    if (g_consoles != 1 || g_pConsole->Count != 4) ok = 0;
    std::printf("006bbc30_TEST %s\n", ok ? "PASS" : "FAIL");
    return ok ? 0 : 1;
}