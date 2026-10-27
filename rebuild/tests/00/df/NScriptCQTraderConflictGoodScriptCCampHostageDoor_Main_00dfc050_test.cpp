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
struct Thing { int usedAfter; int polls;
    bool MsgIsUsedBy(const char* who) { ++polls; return std::strcmp(who, "SCRIPT_NAME_HERO") == 0 && polls > usedAfter; } };
struct Parent { bool OpenedCage; };
struct Game {
    void NewScriptFrame() { Log("frame"); }
    void CloseDoor(const Thing&) { Log("close"); }
    void OpenDoor(const Thing&) { Log("open"); }
};
struct Door { Game* G; Thing Me; Parent* ParentClass;
    bool IsActiveThreadTerminating() { return Term(); }
    void Main()
    {
        G->CloseDoor(Me);
        if (IsActiveThreadTerminating())
            return;
        do {
            if (Me.MsgIsUsedBy("SCRIPT_NAME_HERO")) {
                if (IsActiveThreadTerminating())
                    return;
                G->OpenDoor(Me);
                ParentClass->OpenedCage = true;
                return;
            }
            G->NewScriptFrame();
        } while (!IsActiveThreadTerminating());
    }
};
int main()
{
    bool ok = true;
    Game g; Parent p; p.OpenedCage = false;
    Door d; d.G = &g; d.ParentClass = &p; d.Me.usedAfter = 2; d.Me.polls = 0;
    Reset(1000); d.Main();
    ok = std::strcmp(g_log, "close;frame;frame;open;") == 0 && p.OpenedCage;
    p.OpenedCage = false; d.Me.polls = 0;
    Reset(2); d.Main();
    ok = ok && std::strcmp(g_log, "close;frame;frame;") == 0 && !p.OpenedCage;
    if (!ok) { std::printf("log: %s\n", g_log); std::printf("00dfc050_TEST FAIL\n"); return 1; }
    std::printf("00dfc050_TEST PASS\n");
    return 0;
}