#include <cstdio>
// behaviour test for CThingBuilding::WorldUpdate (0x0082D810)
unsigned char g_displaySwitch_13b8637 = 0;
long g_displayMode_13b8638 = 0;
unsigned char g_displaySwitch_13b863d = 0;
unsigned char g_displaySwitch_13b863c = 0;
unsigned char g_displaySwitch_13b863e = 0;
unsigned char g_displaySwitch_13b86c6 = 0;
static bool g_baseResult = false;
static int g_updates = 0;
static long g_reason = 0;
struct CThingGameObject {
    unsigned long m_thingFlags;
    bool WorldUpdate() { return g_baseResult; }
    void UpdateGraphicAppearance(long reason) { ++g_updates; g_reason = reason; }
};
struct CThingBuilding : CThingGameObject {
    bool WorldUpdate();
};
bool CThingBuilding::WorldUpdate()
{
    if (CThingGameObject::WorldUpdate())
        return true;
    if ((m_thingFlags & 0x400) || g_displaySwitch_13b8637 || g_displayMode_13b8638 || g_displaySwitch_13b863d
        || g_displaySwitch_13b863c || g_displaySwitch_13b863e || g_displaySwitch_13b86c6)
        UpdateGraphicAppearance(4);
    return false;
}
int main()
{
    CThingBuilding b; b.m_thingFlags = 0;
    g_baseResult = true;
    if (!b.WorldUpdate() || g_updates != 0) { std::printf("0082d810_TEST FAIL\n"); return 1; }
    g_baseResult = false;
    if (b.WorldUpdate() || g_updates != 0) { std::printf("0082d810_TEST FAIL\n"); return 1; }
    b.m_thingFlags = 0x400;
    if (b.WorldUpdate() || g_updates != 1 || g_reason != 4) { std::printf("0082d810_TEST FAIL\n"); return 1; }
    b.m_thingFlags = 0x3ff; g_displaySwitch_13b86c6 = 1;
    if (b.WorldUpdate() || g_updates != 2) { std::printf("0082d810_TEST FAIL\n"); return 1; }
    g_displaySwitch_13b86c6 = 0; g_displayMode_13b8638 = 2;
    if (b.WorldUpdate() || g_updates != 3) { std::printf("0082d810_TEST FAIL\n"); return 1; }
    std::printf("0082d810_TEST PASS\n");
    return 0;
}