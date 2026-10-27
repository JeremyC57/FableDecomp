#include <cstdio>
#include <cstring>
// Behaviour test: a copy of the source's logic over a mock model (calls are recorded, not executed).
static char g_log[2048];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
static const char* g_str = "";
struct Script {
    int ParanoidState;
    bool HideDiggingSpot;
    int Tail;

    void Init()
    {
        ParanoidState = 1; HideDiggingSpot = true;
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
    ok = (s.ParanoidState == 1 && s.HideDiggingSpot) && s.Tail == 77;

    if (!ok) { std::printf("00ebeec0_TEST FAIL\n"); return 1; }
    std::printf("00ebeec0_TEST PASS\n");
    return 0;
}