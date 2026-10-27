#include <cstdio>
#include <cstring>
// Behaviour test: a copy of the source's logic over a mock model (calls are recorded, not executed).
static char g_log[2048];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
static const char* g_str = "";
struct Script {
    bool WifeNeededForCutscene;
    bool HeroKnowsAboutNecklace;
    bool NecklaceReturned;
    bool BanditKilled;
    bool WarnedGettingCold;
    bool GuidedToBanditLocation;
    bool GhostToRest;
    bool QuestCardGiven;
    int Tail;

    void Init()
    {
        WifeNeededForCutscene = false; HeroKnowsAboutNecklace = false; NecklaceReturned = false; BanditKilled = false;
        WarnedGettingCold = false; GuidedToBanditLocation = false; GhostToRest = false; QuestCardGiven = false;
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
    ok = (!s.WifeNeededForCutscene && !s.HeroKnowsAboutNecklace && !s.NecklaceReturned && !s.BanditKilled && !s.WarnedGettingCold && !s.GuidedToBanditLocation && !s.GhostToRest && !s.QuestCardGiven) && s.Tail == 77;

    if (!ok) { std::printf("00e8b880_TEST FAIL\n"); return 1; }
    std::printf("00e8b880_TEST PASS\n");
    return 0;
}