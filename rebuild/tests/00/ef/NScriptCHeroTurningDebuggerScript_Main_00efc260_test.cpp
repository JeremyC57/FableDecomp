#include <cstdio>
#include <cstring>
// Behaviour test: a copy of the source's logic over a mock model (interface calls are logged, not executed).
static char g_log[4096];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
static void LogI(const char* s, int v) { char b[128]; std::sprintf(b, "%s%d", s, v); Log(b); }
static void LogS(const char* s, const char* v) { char b[256]; std::sprintf(b, "%s %s", s, v); Log(b); }
static int g_termAfter = 1000;   // IsActiveThreadTerminating() turns true after this many calls
static int g_termCalls = 0;
static bool Term() { return ++g_termCalls > g_termAfter; }
static void Reset(int termAfter) { g_log[0] = 0; g_termCalls = 0; g_termAfter = termAfter; }
struct Script;
struct Spawned { const char* Name; int Id; Script* PClass; void (Script::*Func)();
    Spawned(const char* n, int id, Script* p, void (Script::*f)()) : Name(n), Id(id), PClass(p), Func(f) { }
    void Run() { (PClass->*Func)(); } };
static Spawned* g_spawned;
struct Game { void PostAddScriptedEntities() { Log("game-postadd"); } };
struct Script { Game* G;
    void PostAddScriptedEntities() { Log("postadd"); }
    void AddSpawnedFunction(Spawned* f, const char* region) { LogS("spawn", f->Name); LogI("region", (int)std::strlen(region)); g_spawned = f; }
    void SpawnFunction(Spawned* f, const char* region = "") { AddSpawnedFunction(f, region); }
    void MoveHeroTest() { Log("move"); }
    void Main()
    {
        PostAddScriptedEntities();
        G->PostAddScriptedEntities();
        SpawnFunction(new Spawned("MoveHeroTest", 0, this, &Script::MoveHeroTest));
    }
};
int main()
{
    bool ok = true;
    Game g; Script s; s.G = &g;
    Reset(0); s.Main();
    ok = ok && std::strcmp(g_log, "postadd;game-postadd;spawn MoveHeroTest;region0;") == 0;
    Reset(0); g_spawned->Run(); ok = ok && std::strcmp(g_log, "move;") == 0;
    delete g_spawned;
    if (!ok) { std::printf("log: %s\n", g_log); std::printf("00efc260_TEST FAIL\n"); return 1; }
    std::printf("00efc260_TEST PASS\n");
    return 0;
}