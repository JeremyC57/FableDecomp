// Behaviour test for CTCMultiArrow::Init (0x007CC4D0): spell-level indexed shot counts, volley count, the ammo-effect
// delay in frames (fps * seconds, truncated) stored to both delay and timer, and the conditional cast effect / sound.
#include <cstdio>
#include <cstring>

static char g_log[256];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
static long g_FramesPerSecond = 15;

struct Def { long NumberOfShots[4]; long NumberInVolley[4]; float Delay; bool hasCastEffect; };
struct MultiArrow {
    Def* PDef;
    long NumberOfShots, NumberInVolley, NumOfVolleys, SpellLevel, Count, Delay, Timer;
    bool hasAbilities, hasSound;
    void Init(long level)
    {
        if (hasAbilities) SpellLevel = level;
        NumberOfShots = PDef->NumberOfShots[SpellLevel];
        NumberInVolley = PDef->NumberInVolley[SpellLevel];
        NumOfVolleys = NumberOfShots / NumberInVolley;
        Count = NumOfVolleys;
        Delay = Timer = (long)(g_FramesPerSecond * PDef->Delay);
        Log("noise"); Log("weapon"); Log("ammo");
        if (PDef->hasCastEffect) Log("cast");
        if (hasSound) Log("sound");
    }
};

int main()
{
    Def d = { { 3, 6, 9, 12 }, { 1, 2, 3, 4 }, 0.5f, true };
    MultiArrow m; std::memset(&m, 0, sizeof(m)); m.PDef = &d; m.hasAbilities = true; m.hasSound = true;
    g_log[0] = 0; m.Init(2);
    if (m.NumberOfShots != 9 || m.NumberInVolley != 3 || m.NumOfVolleys != 3 || m.Count != 3 || m.Delay != 7 || m.Timer != 7 ||
        std::strcmp(g_log, "noise;weapon;ammo;cast;sound;") != 0) { std::printf("007cc4d0_TEST FAIL\n"); return 1; }
    d.hasCastEffect = false; m.hasAbilities = false; m.hasSound = false; g_log[0] = 0; m.Init(0);
    if (m.SpellLevel != 2 || std::strcmp(g_log, "noise;weapon;ammo;") != 0) { std::printf("007cc4d0_TEST FAIL\n"); return 1; }
    std::printf("007cc4d0_TEST PASS\n");
    return 0;
}
