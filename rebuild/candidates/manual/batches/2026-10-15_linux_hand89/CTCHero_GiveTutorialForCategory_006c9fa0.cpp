// CTCHero::GiveTutorialForCategory(ETutorialCategory, bool) -- asks the user-profile manager singleton to give the tutorial for a category
// Leaf: GiveTutorialForCategory
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006C9FA0, length 25 bytes (+ padding). Ego_r.pdb:
// ?GiveTutorialForCategory@CTCHero@@QAE_NW4ETutorialCategory@@_N@Z (the manifest's label for this one is right; its
// propagated copies 0x00572672 / 0x00572689 / 0x00B2AD90 are other functions).
// Callees: CTBaseSingleton<CUserProfileManager>::Get @ 0x0040D2A0 (no arguments; `this` unused), CUserProfileManager::
// GiveTutorialForCategory(ETutorialCategory, bool) @ 0x0040B790.
// Shape: the speed-optimised form loads both arguments into eax/ecx and pushes them BEFORE the singleton call (arguments are
// evaluated before the object expression); the result is returned in al untouched; `ret 8`.
// Local model: no engine header exists for CTCHero.
enum ETutorialCategory { TUTORIAL_CATEGORY_NULL };

struct CUserProfileManager {
    bool GiveTutorialForCategory(ETutorialCategory category, bool force);   // 0x0040B790
};
CUserProfileManager& __fastcall GetUserProfileManager();   // 0x0040D2A0 CTBaseSingleton<CUserProfileManager>::Get

struct CTCHero {
    bool GiveTutorialForCategory(ETutorialCategory category, bool force);
};

bool CTCHero::GiveTutorialForCategory(ETutorialCategory category, bool force)
{
    return GetUserProfileManager().GiveTutorialForCategory(category, force);
}
