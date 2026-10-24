// behaviour test for 00d15170: TeleportData's implicit operator= -- every member copied (the four
// strings through CCharString::operator=, the seed and health as words), returning *this.
#include <cstdio>
#include <cstring>
static int g_assigns = 0;
struct CCharString {
    char Text[16];
    CCharString() { Text[0] = 0; }
    CCharString(const CCharString& o) { std::strcpy(Text, o.Text); }
    CCharString& operator=(const CCharString& o) { ++g_assigns; std::strcpy(Text, o.Text); return *this; }
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
    std::strcpy(a.defName.Text, "CREATURE_BANDIT");
    a.appearanceSeed = 1234;
    a.health = 55.5f;
    std::strcpy(a.scriptName.Text, "Bandit01");
    std::strcpy(a.teleportTo.Text, "Region2");
    std::strcpy(a.teleportMkr.Text, "Marker7");
    TeleportData b;
    b.appearanceSeed = 0;
    b.health = 0.0f;
    TeleportData& r = (b = a);
    int ok = 1;
    if (&r != &b || g_assigns != 4) ok = 0;
    if (std::strcmp(b.defName.Text, "CREATURE_BANDIT") || std::strcmp(b.scriptName.Text, "Bandit01") || std::strcmp(b.teleportTo.Text, "Region2") || std::strcmp(b.teleportMkr.Text, "Marker7")) ok = 0;
    if (b.appearanceSeed != 1234 || b.health != 55.5f) ok = 0;
    if (!ok) { std::printf("00d15170_TEST FAIL\n"); return 1; }
    std::printf("00d15170_TEST PASS\n");
    return 0;
}