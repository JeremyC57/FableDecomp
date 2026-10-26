#include <cstdio>

class CUserProfileManager { public:
    char pad0[0xc];
    char flag0c;
    char pad_d[0x30-0xd];
    char UsingHeadPhones;
    char pad_31[0xf0-0x31];
    char UsingHeadPhonesNewProfile;
};

int g_n1 = 0;
int g_n2 = 0;

void __fastcall CUPM_Notify1(CUserProfileManager* manager) { (void)manager; g_n1++; }
void __fastcall CUPM_Notify2(CUserProfileManager* manager) { (void)manager; g_n2++; }

void __fastcall CUserProfileManager_SetUsingHeadPhones(CUserProfileManager* manager, void* edx, char p1, char p2)
{
    (void)edx;
    if (manager->flag0c)
        manager->UsingHeadPhonesNewProfile = p1;
    manager->UsingHeadPhones = p1;
    CUPM_Notify1(manager);
    if (p2)
        CUPM_Notify2(manager);
}

int main()
{
    CUserProfileManager m;
    m.flag0c = 0; m.UsingHeadPhones = 0; m.UsingHeadPhonesNewProfile = 0;
    g_n1 = 0; g_n2 = 0;
    CUserProfileManager_SetUsingHeadPhones(&m, 0, 1, 0);
    if (m.UsingHeadPhonesNewProfile != 0) { std::printf("FAIL flagf0 set when flag0c=0\n"); return 1; }
    if (m.UsingHeadPhones != 1) { std::printf("FAIL flag30\n"); return 1; }
    if (g_n1 != 1) { std::printf("FAIL notify1\n"); return 1; }
    if (g_n2 != 0) { std::printf("FAIL notify2 called\n"); return 1; }

    m.flag0c = 1; m.UsingHeadPhones = 0; m.UsingHeadPhonesNewProfile = 0;
    g_n1 = 0; g_n2 = 0;
    CUserProfileManager_SetUsingHeadPhones(&m, 0, 1, 1);
    if (m.UsingHeadPhonesNewProfile != 1) { std::printf("FAIL flagf0\n"); return 1; }
    if (m.UsingHeadPhones != 1) { std::printf("FAIL flag30 b\n"); return 1; }
    if (g_n1 != 1) { std::printf("FAIL notify1 b\n"); return 1; }
    if (g_n2 != 1) { std::printf("FAIL notify2 b\n"); return 1; }

    std::printf("CUserProfileManager_0040b410_TEST PASS\n");
    return 0;
}