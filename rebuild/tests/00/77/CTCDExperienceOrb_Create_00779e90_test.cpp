// Behaviour test for 0x00779E90 CTCDExperienceOrb::Create(defIndex, position) -- create, construct, activate or kill
#include <cstdio>

struct Thing {
    bool HasOrbDriver;
    bool ActivationOk;
    bool Killed;
    bool Constructed;
    long DefIndex, Player, Map;
    bool Persistent;
};

static Thing g_pool[4];
static int g_next = 0;
static bool g_refuse = false;

static Thing* CreateThing(long defIndex, long player, long map, bool persistent)
{
    if (g_refuse)
        return 0;
    Thing* t = &g_pool[g_next++];
    t->DefIndex = defIndex; t->Player = player; t->Map = map; t->Persistent = persistent;
    return t;
}
static long MapNumberAt(float x) { return (long)x / 100; }

static Thing* CreateOrb(long defIndex, float x)
{
    Thing* thing = CreateThing(defIndex, 4, MapNumberAt(x), true);
    Thing* result = 0;
    if (thing) {
        if (thing->HasOrbDriver) {
            thing->Constructed = true;
            if (thing->ActivationOk)
                result = thing;
        }
        else
            thing->Killed = true;
    }
    return result;
}

int main()
{
    g_pool[0].HasOrbDriver = true;  g_pool[0].ActivationOk = true;
    g_pool[1].HasOrbDriver = true;  g_pool[1].ActivationOk = false;
    g_pool[2].HasOrbDriver = false; g_pool[2].ActivationOk = true;
    Thing* a = CreateOrb(77, 250.0f);
    if (a != &g_pool[0] || !a->Constructed || a->Killed || a->Player != 4 || a->Map != 2 || !a->Persistent || a->DefIndex != 77) { std::printf("00779e90_TEST FAIL\n"); return 1; }
    if (CreateOrb(77, 0.0f) != 0 || !g_pool[1].Constructed || g_pool[1].Killed) { std::printf("00779e90_TEST FAIL\n"); return 1; }
    if (CreateOrb(77, 0.0f) != 0 || g_pool[2].Constructed || !g_pool[2].Killed) { std::printf("00779e90_TEST FAIL\n"); return 1; }
    g_refuse = true;
    if (CreateOrb(77, 0.0f) != 0) { std::printf("00779e90_TEST FAIL\n"); return 1; }
    std::printf("00779e90_TEST PASS\n");
    return 0;
}