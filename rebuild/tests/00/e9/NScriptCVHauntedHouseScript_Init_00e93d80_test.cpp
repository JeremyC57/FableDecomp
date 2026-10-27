#include <cstdio>
#include <cstring>
// Behaviour test: a copy of the source's logic over a mock model (calls are recorded, not executed).
static char g_log[2048];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
static const char* g_str = "";
struct Script {
    float UndeadBar;
    int GuiCounter;
    bool CellarDoorOpen;
    bool HeroBeenToChest;
    bool QuestCardGiven;
    bool InHouse;
    int UndeadAlive;
    int MaxUndead;
    int Tail;

    void Init()
    {
        UndeadBar = 0.0f; GuiCounter = 0; CellarDoorOpen = false; HeroBeenToChest = false; QuestCardGiven = false;
        InHouse = false; UndeadAlive = 0; MaxUndead = 4;
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
    ok = (s.UndeadBar == 0.0f && s.GuiCounter == 0 && !s.CellarDoorOpen && !s.HeroBeenToChest && !s.QuestCardGiven && !s.InHouse && s.UndeadAlive == 0 && s.MaxUndead == 4) && s.Tail == 77;

    if (!ok) { std::printf("00e93d80_TEST FAIL\n"); return 1; }
    std::printf("00e93d80_TEST PASS\n");
    return 0;
}