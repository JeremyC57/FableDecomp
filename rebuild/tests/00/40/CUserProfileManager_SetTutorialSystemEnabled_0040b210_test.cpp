#include <cstdio>

class CUserProfileManager { public:
    char pad0[0xc];
    char UseNewProfileValues;      // +0xc
    char pad1[0xa];
    char TutorialSystemEnabled;     // +0x17
    char pad2[0xbf];
    char TutorialSystemEnabledNewProfile;     // +0xd7
};

static int g_h1 = 0;
static int g_h2 = 0;

void __fastcall CUserProfileManager_Helper1(CUserProfileManager* manager) { g_h1++; }
void __fastcall CUserProfileManager_Helper2(CUserProfileManager* manager) { g_h2++; }

void __fastcall CUserProfileManager_SetTutorialSystemEnabled(CUserProfileManager* manager, int edx_dummy, char p1, char p2)
{
    if (manager->UseNewProfileValues)
        manager->TutorialSystemEnabledNewProfile = p1;
    manager->TutorialSystemEnabled = p1;
    CUserProfileManager_Helper1(manager);
    if (p2)
        CUserProfileManager_Helper2(manager);
}

int main()
{
    CUserProfileManager o;
    for (int i = 0; i < (int)sizeof(o); i++) ((char*)&o)[i] = 0;

    // Case: flagC set, p2 set
    o.UseNewProfileValues = 1;
    g_h1 = g_h2 = 0;
    CUserProfileManager_SetTutorialSystemEnabled(&o, 0, 5, 7);
    if (o.TutorialSystemEnabledNewProfile != 5) { std::printf("FAIL d7\n"); return 1; }
    if (o.TutorialSystemEnabled != 5) { std::printf("FAIL 17\n"); return 1; }
    if (g_h1 != 1) { std::printf("FAIL h1\n"); return 1; }
    if (g_h2 != 1) { std::printf("FAIL h2\n"); return 1; }

    // Case: flagC clear, p2 clear
    o.UseNewProfileValues = 0;
    o.TutorialSystemEnabledNewProfile = 99;
    g_h1 = g_h2 = 0;
    CUserProfileManager_SetTutorialSystemEnabled(&o, 0, 3, 0);
    if (o.TutorialSystemEnabledNewProfile != 99) { std::printf("FAIL d7-2\n"); return 1; }
    if (o.TutorialSystemEnabled != 3) { std::printf("FAIL 17-2\n"); return 1; }
    if (g_h1 != 1) { std::printf("FAIL h1-2\n"); return 1; }
    if (g_h2 != 0) { std::printf("FAIL h2-2\n"); return 1; }

    std::printf("CUserProfileManager_0040b210_TEST PASS\n");
    return 0;
}