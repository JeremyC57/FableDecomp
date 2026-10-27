#include <cstdio>
#include <cstring>
// Behaviour test: a copy of the source's logic over a mock model (calls are recorded, not executed).
static char g_log[2048];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
static const char* g_str = "";
struct Script {
    bool HaveTalked;
    bool HeroHasPlayed;
    bool KickingChickens;
    bool GhostChat;
    int Tail;

    void Init()
    {
        HeroHasPlayed = false; HaveTalked = false; KickingChickens = false; GhostChat = false;
    }
};

int main()
{
    g_log[0] = 0;
    bool ok = true;
    Script s;
    std::memset(&s, 0x5a, sizeof(s));
    s.Tail = 77;
    s.Init();
    ok = (!s.HaveTalked && !s.HeroHasPlayed && !s.KickingChickens && !s.GhostChat) && s.Tail == 77;

    if (!ok) { std::printf("00e63560_TEST FAIL\n"); return 1; }
    std::printf("00e63560_TEST PASS\n");
    return 0;
}