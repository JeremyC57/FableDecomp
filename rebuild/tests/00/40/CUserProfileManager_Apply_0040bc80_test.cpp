#include <cstdio>
struct CUserProfileManager { char pad[0x33]; char flag; };
static int g_prep = 0;
static int g_commit = 0;
extern "C" void __fastcall UPM_Prep(CUserProfileManager* manager) { (void)manager; g_prep++; }
extern "C" void __fastcall UPM_Commit(CUserProfileManager* manager) { (void)manager; g_commit++; }

void __fastcall CUserProfileManager_Apply(CUserProfileManager* manager)
{
    UPM_Prep(manager);
    if (manager->flag)
        UPM_Commit(manager);
}

int main()
{
    CUserProfileManager a;
    a.flag = 1;
    g_prep = g_commit = 0;
    CUserProfileManager_Apply(&a);
    if (g_prep != 1 || g_commit != 1) { std::printf("FAIL branch-taken prep=%d commit=%d\n", g_prep, g_commit); return 1; }

    CUserProfileManager b;
    b.flag = 0;
    g_prep = g_commit = 0;
    CUserProfileManager_Apply(&b);
    if (g_prep != 1 || g_commit != 0) { std::printf("FAIL branch-skip prep=%d commit=%d\n", g_prep, g_commit); return 1; }

    std::printf("CUserProfileManager_0040bc80_TEST PASS\n");
    return 0;
}