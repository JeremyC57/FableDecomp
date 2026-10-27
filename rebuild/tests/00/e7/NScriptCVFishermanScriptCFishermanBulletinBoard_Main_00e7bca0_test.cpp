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
struct Thing { int Id; };
struct Parent { bool HeroHasSavedFishermanFromWasps; };
static Parent* g_p; static int g_frames;
struct Game {
    void NewScriptFrame() { Log("frame"); if (++g_frames == 2) g_p->HeroHasSavedFishermanFromWasps = true; }
    void SetReadableObjectTextTag(const Thing&, const char* t) { LogS("tag", t); }
};
struct Board { Game* G; Thing Me; Parent* ParentClass;
    bool IsActiveThreadTerminating() { return Term(); }
    void Main()
    {
        G->SetReadableObjectTextTag(Me, "TEXT_QST_023_SIGN_WASP_PLAGUE");
        while (!ParentClass->HeroHasSavedFishermanFromWasps) {
            G->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        G->SetReadableObjectTextTag(Me, "TEXT_QST_023_SIGN_COME_FISHING");
    }
};
int main()
{
    bool ok = true;
    Game g; Parent p; p.HeroHasSavedFishermanFromWasps = false; g_p = &p; g_frames = 0;
    Board b; b.G = &g; b.ParentClass = &p;
    Reset(1000); b.Main();
    ok = std::strcmp(g_log, "tag TEXT_QST_023_SIGN_WASP_PLAGUE;frame;frame;tag TEXT_QST_023_SIGN_COME_FISHING;") == 0;
    p.HeroHasSavedFishermanFromWasps = false; g_frames = 5;
    Reset(1); b.Main();
    ok = ok && std::strcmp(g_log, "tag TEXT_QST_023_SIGN_WASP_PLAGUE;frame;frame;") == 0;
    if (!ok) { std::printf("log: %s\n", g_log); std::printf("00e7bca0_TEST FAIL\n"); return 1; }
    std::printf("00e7bca0_TEST PASS\n");
    return 0;
}