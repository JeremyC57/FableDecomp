#include <cstdio>
#include <cstring>
// Behaviour test: a copy of the source's logic over a mock model (calls are recorded, not executed).
static char g_log[2048];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
static const char* g_str = "";
struct Script {
    bool GuildMasterDialogue[5];
    void* LastDialogueSaid;
    int Tail;

    void Init()
    {
        for (int i = 0; i < 5; ++i)
            GuildMasterDialogue[i] = false;
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
    ok = (!s.GuildMasterDialogue[0] && !s.GuildMasterDialogue[1] && !s.GuildMasterDialogue[2] && !s.GuildMasterDialogue[3] && !s.GuildMasterDialogue[4]) && s.Tail == 77;

    if (!ok) { std::printf("00e90780_TEST FAIL\n"); return 1; }
    std::printf("00e90780_TEST PASS\n");
    return 0;
}