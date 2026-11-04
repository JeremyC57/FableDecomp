// Behaviour test for CTCHero::FrameUpdate 0x006C9FD0: each hero frame update hands pending tutorials to the user profile manager
// singleton (CUserProfileManager::Get().ProcessPendingTutorials()); the hero component itself is untouched.
#include <cstdio>
struct CUserProfileManager {
    int PendingTutorials, Processed;
    static CUserProfileManager& Get() { static CUserProfileManager instance = { 0, 0 }; return instance; }
    void ProcessPendingTutorials() { Processed += PendingTutorials; PendingTutorials = 0; }
};
struct CTCBase { virtual ~CTCBase() {} virtual void FrameUpdate() {} };
struct CTCHero : CTCBase { int Title; void FrameUpdate() { CUserProfileManager::Get().ProcessPendingTutorials(); } };
int main()
{
    CTCHero hero; hero.Title = 3;
    CTCBase* tc = &hero;
    CUserProfileManager::Get().PendingTutorials = 2;
    tc->FrameUpdate();
    if (CUserProfileManager::Get().Processed != 2 || CUserProfileManager::Get().PendingTutorials != 0 || hero.Title != 3) { std::printf("006c9fd0_TEST FAIL\n"); return 1; }
    tc->FrameUpdate();
    if (CUserProfileManager::Get().Processed != 2) { std::printf("006c9fd0_TEST FAIL\n"); return 1; }
    std::printf("006c9fd0_TEST PASS\n");
    return 0;
}