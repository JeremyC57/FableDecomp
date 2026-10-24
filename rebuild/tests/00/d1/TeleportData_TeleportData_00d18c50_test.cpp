// behaviour test for 00d18c50: TeleportData's implicit copy constructor -- the four strings are
// copy-constructed, the seed and health copied as words.
#include <cstdio>
#include <cstring>
static int g_copies = 0;
struct CCharString {
    char Text[16];
    CCharString() { Text[0] = 0; }
    CCharString(const CCharString& o) { ++g_copies; std::strcpy(Text, o.Text); }
    CCharString& operator=(const CCharString& o) { std::strcpy(Text, o.Text); return *this; }
};
struct TeleportData {
    CCharString defName;
    unsigned long appearanceSeed;
    float health;
    CCharString scriptName;
    CCharString teleportTo;
    CCharString teleportMkr;
};
int main()
{
    TeleportData a;
    std::strcpy(a.defName.Text, "CREATURE_GUARD");
    a.appearanceSeed = 99;
    a.health = 12.25f;
    std::strcpy(a.scriptName.Text, "Guard");
    std::strcpy(a.teleportTo.Text, "Bowerstone");
    std::strcpy(a.teleportMkr.Text, "Gate");
    TeleportData b(a);
    int ok = 1;
    if (g_copies != 4) ok = 0;
    if (std::strcmp(b.defName.Text, "CREATURE_GUARD") || std::strcmp(b.scriptName.Text, "Guard") || std::strcmp(b.teleportTo.Text, "Bowerstone") || std::strcmp(b.teleportMkr.Text, "Gate")) ok = 0;
    if (b.appearanceSeed != 99 || b.health != 12.25f) ok = 0;
    if (!ok) { std::printf("00d18c50_TEST FAIL\n"); return 1; }
    std::printf("00d18c50_TEST PASS\n");
    return 0;
}