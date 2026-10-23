// Behaviour test for CTCInventoryTrade::CloseInventory @ 0x0059f304: call order, component deletion, volume pick, release, interface gate
#include <cstdio>
#include <cstring>
static char g_log[512];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ","); }
static int g_deleted = 0;
struct Component { virtual ~Component() { ++g_deleted; } };
struct Profile { bool custom; float sfx, music, customSfx, customMusic;
    float GetSFX() const { return custom ? customSfx : sfx; }
    float GetMusic() const { return custom ? customMusic : music; } };
static Profile g_profile;
static float g_vol[3];
static long g_cat[3];
static int g_volCalls = 0;
static void Refresh(long c, float v, float z) { if (z != 0.0f) return; g_cat[g_volCalls] = c; g_vol[g_volCalls] = v; ++g_volCalls; }
struct Data { long RefCount; int deleted; void Release() { if (--RefCount == 0) deleted = 1; } };
struct Trade {
    Component* comps[4]; unsigned long count;
    long previousScreen; long screenType; bool open, initialised; long closeFrame;
    bool hasWeapons;
    void Close(bool pc, Data* player, bool sound)
    {
        if (!pc) { screenType = previousScreen; Log("screen"); }
        Log("special"); Log("base");
        if (pc) Log("delcomp");
        for (unsigned long i = 0; i < count; ++i) delete comps[i];
        count = 0;
        Log("maps"); Log("fe");
        closeFrame = 77;
        if (sound) {
            Refresh(0x10, g_profile.GetSFX(), 0.0f);
            Refresh(0x40, g_profile.GetMusic(), 0.0f);
            Refresh(0x80, g_profile.GetSFX(), 0.0f);
        }
        if (player) player->Release();
        if (hasWeapons) Log("weapons");
        if (pc) { open = false; initialised = false; } else Log("closemenus");
    }
};
int main()
{
    int ok = 1;
    bool pc = false;
    Trade t; t.count = 2; t.comps[0] = new Component; t.comps[1] = 0; t.previousScreen = 5; t.screenType = 0;
    t.open = true; t.initialised = true; t.hasWeapons = true;
    g_profile.custom = true; g_profile.sfx = 1; g_profile.music = 2; g_profile.customSfx = 3; g_profile.customMusic = 4;
    Data d; d.RefCount = 1; d.deleted = 0;
    g_log[0] = 0;
    t.Close(pc, &d, true);
    if (g_deleted != 1 || t.count != 0 || d.deleted != 1 || d.RefCount != 0) ok = 0;
    if (g_volCalls != 3 || g_vol[0] != 3 || g_vol[1] != 4 || g_vol[2] != 3 || g_cat[1] != 0x40) ok = 0;
    if (t.closeFrame != 77) ok = 0;
    if (pc) { if (t.open || t.initialised || std::strcmp(g_log, "special,base,delcomp,maps,fe,weapons,") != 0) ok = 0; }
    else { if (t.screenType != 5 || std::strcmp(g_log, "screen,special,base,maps,fe,weapons,closemenus,") != 0) ok = 0; }
    if (!ok) { std::printf("0059f304_TEST FAIL\n"); return 1; }
    std::printf("0059f304_TEST PASS\n");
    return 0;
}
