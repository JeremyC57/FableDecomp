#include <cstdio>

// Model of CTCHero::GiveTutorialForCategory @ 0x006C9FA0: forwards to the user-profile manager singleton.
class CUserProfileManager { public:
    int lastCategory; bool lastForce; int given;
    bool GiveTutorialForCategory(int category, bool force)
    {
        lastCategory = category; lastForce = force;
        if (force || (given & (1 << category)) == 0) { given |= 1 << category; return true; }
        return false;
    }
public:
    static CUserProfileManager& __fastcall Get();   // 0x0040D2A0
};
static CUserProfileManager g_profile = { -1, false, 0 };
CUserProfileManager& CUserProfileManager::Get() { return g_profile; }
class CTCHero { public:
    bool GiveTutorialForCategory(int category, bool force) { return CUserProfileManager::Get().GiveTutorialForCategory(category, force); }
};

int main()
{
    CTCHero hero;
    if (!hero.GiveTutorialForCategory(3, false) || g_profile.lastCategory != 3 || g_profile.lastForce) { std::printf("006c9fa0_TEST FAIL\n"); return 1; }
    if (hero.GiveTutorialForCategory(3, false)) { std::printf("006c9fa0_TEST FAIL\n"); return 1; }
    if (!hero.GiveTutorialForCategory(3, true) || !g_profile.lastForce) { std::printf("006c9fa0_TEST FAIL\n"); return 1; }
    std::printf("006c9fa0_TEST PASS\n");
    return 0;
}