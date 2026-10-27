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
struct Def { long TH_ThunderConv_Length; };
static Def g_def = { 3 };
static Def* pGlobalScriptDef = &g_def;
struct Script;
static Script* g_s; static int g_frames;
struct Game { void NewScriptFrame(); };
struct Script { Game* G; bool ThunderKidsTalking, ThunderSceneStarted; int lines;
    bool IsActiveThreadTerminating() { return Term(); }
    void DoConversation(long line) { LogI("line", line); if (++lines == 5) ThunderSceneStarted = true; }
    void ThunderChatter()
    {
        long line = 0;
        long length = pGlobalScriptDef->TH_ThunderConv_Length;
        while (!ThunderSceneStarted) {
            G->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
            if (ThunderKidsTalking) {
                if (IsActiveThreadTerminating())
                    return;
                while (ThunderKidsTalking) {
                    G->NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return;
                }
                if (IsActiveThreadTerminating())
                    return;
            }
            DoConversation(line);
            ++line;
            if (line == length) {
                if (IsActiveThreadTerminating())
                    return;
                line = 0;
            }
        }
        if (IsActiveThreadTerminating())
            return;
    }
};
void Game::NewScriptFrame() { Log("frame"); ++g_frames; if (g_frames == 3) g_s->ThunderKidsTalking = true; if (g_frames == 5) g_s->ThunderKidsTalking = false; }
int main()
{
    bool ok = true;
    Game g; Script s; s.G = &g; g_s = &s; g_frames = 0; s.lines = 0; s.ThunderKidsTalking = false; s.ThunderSceneStarted = false;
    Reset(1000); s.ThunderChatter();
    ok = ok && std::strcmp(g_log, "frame;line0;frame;line1;frame;frame;frame;line2;frame;line0;frame;line1;") == 0;
    s.lines = 0; s.ThunderSceneStarted = false; g_frames = 10;
    Reset(1); s.ThunderChatter(); ok = ok && std::strcmp(g_log, "frame;line0;frame;") == 0;
    if (!ok) { std::printf("log: %s\n", g_log); std::printf("00ee29d0_TEST FAIL\n"); return 1; }
    std::printf("00ee29d0_TEST PASS\n");
    return 0;
}