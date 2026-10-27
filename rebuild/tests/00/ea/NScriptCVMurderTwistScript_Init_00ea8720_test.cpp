#include <cstdio>
#include <cstring>
// Behaviour test: a copy of the source's logic over a mock model (calls are recorded, not executed).
static char g_log[2048];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
static const char* g_str = "";
struct Script {
    int MurderTwistItemsFound;
    int MurderTwistState;
    int MurderTwistEvent;
    int Tail;

    void Init()
    {
        MurderTwistItemsFound = 0; MurderTwistState = 1; MurderTwistEvent = 1;
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
    ok = (s.MurderTwistItemsFound == 0 && s.MurderTwistState == 1 && s.MurderTwistEvent == 1) && s.Tail == 77;

    if (!ok) { std::printf("00ea8720_TEST FAIL\n"); return 1; }
    std::printf("00ea8720_TEST PASS\n");
    return 0;
}