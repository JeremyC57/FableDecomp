#include <cstdio>
// Behaviour test for 00e0af60: CQ_TraderEscortScript::Alloc builds the script with its interface/data pair, a fresh CommentTimer and an empty TraderToTalk string.
struct Interface { long nextTimer; long RegisterTimer() { return nextTimer++; } };
static Interface g_interface;
static int g_strings = 0;
struct CTimer { long Index; CTimer() : Index(g_interface.RegisterTimer()) {} };
struct CCharString { const char* PStringData; CCharString() : PStringData("") { ++g_strings; } };
struct Base { Interface* Game; void* MasterScriptData; Base(void* data, Interface& game) : Game(&game), MasterScriptData(data) {} };
struct Script : Base {
    CTimer CommentTimer; long a, b; bool f[7]; long c, d, e, g; bool h[6]; long TimesBantered;
    CCharString TraderToTalk;
    Script(void* data, Interface& game) : Base(data, game) {}
};
static Script* Alloc(void* data, Interface& game) { return new Script(data, game); }
int main()
{
    g_interface.nextTimer = 7;
    int token = 0;
    Script* s = Alloc(&token, g_interface);
    int ok = s && s->Game == &g_interface && s->MasterScriptData == &token && s->CommentTimer.Index == 7
          && g_strings == 1 && s->TraderToTalk.PStringData && s->TraderToTalk.PStringData[0] == 0;
    delete s;
    if (!ok) { std::printf("00e0af60_TEST FAIL\n"); return 1; }
    std::printf("00e0af60_TEST PASS\n");
    return 0;
}