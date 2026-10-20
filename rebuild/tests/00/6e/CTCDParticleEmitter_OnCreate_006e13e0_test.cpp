// Behaviour test for CTCDParticleEmitter::OnCreate (0x006E13E0).
#include <cstdio>

static bool g_enabled = true; static bool g_canCreate = false; static int g_persistedOff = 0, g_cleared = 0, g_set = 0;
static long g_fps = 30;
struct Params { long id; };
struct Emitter {
    long ParticleTypeID; long FramesLeft; Params* params;
    bool Waiting, Force, Independant;
    void OnCreate()
    {
        if (!Independant) ++g_persistedOff;
        ++g_cleared;
        if (g_enabled && ParticleTypeID) {
            if (g_canCreate) { ++g_set; return; }
            Waiting = true;
            FramesLeft = (long)(g_fps * 0.9f);
            delete params;
            params = new Params;
            params->id = ParticleTypeID;
        }
    }
};

int main()
{
    Emitter e = { 7, 0, 0, false, false, false };
    e.OnCreate();
    if (g_persistedOff != 1 || g_cleared != 1 || !e.Waiting || e.FramesLeft < 26 || e.FramesLeft > 27 || !e.params || e.params->id != 7) {
        std::printf("006e13e0_TEST FAIL\n"); return 1;
    }
    g_canCreate = true; e.Waiting = false; e.Independant = true; e.OnCreate();
    if (g_set != 1 || e.Waiting || g_persistedOff != 1) { std::printf("006e13e0_TEST FAIL\n"); return 1; }
    e.ParticleTypeID = 0; e.OnCreate();
    if (g_set != 1 || g_cleared != 3) { std::printf("006e13e0_TEST FAIL\n"); return 1; }
    delete e.params;
    std::printf("006e13e0_TEST PASS\n");
    return 0;
}