// Behaviour test for 0x0049F180 (CWorld::ActivateWorld): hero (re)creation only when missing or dead, the stage
// order, and the activation event (type 9, main player, one bool payload byte) plus the start-frame stamp.
#include <cstdio>
#include <cstring>
static char g_log[64]; static int g_n = 0;
static void Log(char c) { g_log[g_n++] = c; g_log[g_n] = 0; }
struct CThing { bool Dead; };
struct CGameEvent {
    long Type; signed char Player; unsigned char Data[32]; unsigned char EndPos; bool Valid; bool Replacement;
    CGameEvent(long type) : Type(type), Player(-1), EndPos(0), Valid(true), Replacement(false) { }
    void AddData(long size, const void* data) { std::memcpy(Data + EndPos, data, size); EndPos = (unsigned char)(EndPos + size); }
    template<class T> void Add(const T& value) { AddData(sizeof(T), &value); }
};
static CThing* g_hero = 0; static bool g_initArg = false; static CGameEvent g_last(0); static long g_worldFrame = 0;
struct World {
    long FrameStarted3DRendering;
    void ActivateWorld(bool loadedGame)
    {
        Log('C');
        CThing* hero = g_hero;
        if (!hero || hero->Dead) { g_initArg = loadedGame; Log('I'); }
        Log('E'); Log('G'); Log('Q'); Log('B');
        CGameEvent event(9);
        event.Add(false);
        event.Player = (signed char)3;
        g_last = event; Log('V');
        FrameStarted3DRendering = g_worldFrame;
    }
};
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d: %s\n", __LINE__, #c); } } while (0)
int main()
{
    World w; w.FrameStarted3DRendering = 0; g_worldFrame = 77;
    w.ActivateWorld(true);
    CHECK(std::strcmp(g_log, "CIEGQBV") == 0); CHECK(g_initArg == true);
    CHECK(g_last.Type == 9 && g_last.Player == 3 && g_last.EndPos == 1 && g_last.Data[0] == 0 && g_last.Valid && !g_last.Replacement);
    CHECK(w.FrameStarted3DRendering == 77);
    CThing hero; hero.Dead = false; g_hero = &hero; g_n = 0;
    w.ActivateWorld(false);
    CHECK(std::strcmp(g_log, "CEGQBV") == 0);
    hero.Dead = true; g_n = 0;
    w.ActivateWorld(false);
    CHECK(std::strcmp(g_log, "CIEGQBV") == 0); CHECK(g_initArg == false);
    if (g_fail) { std::printf("0049f180_TEST FAIL\n"); return 1; }
    std::printf("0049f180_TEST PASS\n");
    return 0;
}
