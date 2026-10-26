#include <cstdio>

class CUserProfileManager { public:
    char pad0[0xc];
    char UseNewProfileValues;                 /* +0x0c */
    char pad1[0x19 - 0x0c - 1];
    char HUDTooltipEnabled;                   /* +0x19 */
    char pad2[0xd9 - 0x19 - 1];
    char HUDTooltipEnabledNewProfile;                   /* +0xd9 */
    void Helper1();
    void Helper2();
    void SetHUDTooltipEnabled(char a, char b);
};

static int g_h1 = 0;
static int g_h2 = 0;

void CUserProfileManager::Helper1() { g_h1++; }
void CUserProfileManager::Helper2() { g_h2++; }

void CUserProfileManager::SetHUDTooltipEnabled(char a, char b)
{
    if (this->UseNewProfileValues)
        this->HUDTooltipEnabledNewProfile = a;
    this->HUDTooltipEnabled = a;
    this->Helper1();
    if (b)
        this->Helper2();
}

int main()
{
    CUserProfileManager o;
    for (int i = 0; i < (int)sizeof(o); i++) ((char*)&o)[i] = 0;

    o.UseNewProfileValues = 1;
    o.SetHUDTooltipEnabled((char)7, (char)1);
    if (o.HUDTooltipEnabledNewProfile != 7) { std::printf("FAIL atD9\n"); return 1; }
    if (o.HUDTooltipEnabled != 7) { std::printf("FAIL at19\n"); return 1; }
    if (g_h1 != 1)   { std::printf("FAIL h1\n"); return 1; }
    if (g_h2 != 1)   { std::printf("FAIL h2\n"); return 1; }

    o.UseNewProfileValues = 0; o.HUDTooltipEnabledNewProfile = 55; o.HUDTooltipEnabled = 0; g_h1 = 0; g_h2 = 0;
    o.SetHUDTooltipEnabled((char)9, (char)0);
    if (o.HUDTooltipEnabledNewProfile != 55) { std::printf("FAIL atD9 preserved\n"); return 1; }
    if (o.HUDTooltipEnabled != 9)  { std::printf("FAIL at19b\n"); return 1; }
    if (g_h1 != 1)    { std::printf("FAIL h1b\n"); return 1; }
    if (g_h2 != 0)    { std::printf("FAIL h2b\n"); return 1; }

    std::printf("CUserProfileManager_0040b290_TEST PASS\n");
    return 0;
}