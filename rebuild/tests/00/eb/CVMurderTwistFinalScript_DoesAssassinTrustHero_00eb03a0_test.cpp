// Behaviour test for CV_MurderTwistFinalScript::DoesAssassinTrustHero (0x00EB03A0).
#include <cstdio>
#include <cstring>
struct CGameScriptInterface {
    const char* outfit;    // "BANDITCAMP", "GUARD" or a mix
    int missing;           // which item (0..4) is not worn, -1 for none
    long morality;
    int GetHero() { return 1; }
    bool IsWearingClothingItem(int hero, const char* item)
    {
        static const char* kinds[] = { "SHIRT", "TROUSERS", "HAT", "BOOTS", "GLOVES" };
        for (int i = 0; i < 5; ++i) {
            char name[64];
            std::sprintf(name, "OBJECT_HERO_%s_%s", kinds[i], outfit);
            if (std::strcmp(name, item) == 0) return i != missing;
        }
        return false;
    }
    long GetHeroMoralityCategory() { return morality; }
};
struct CV_MurderTwistFinalScript {
    CGameScriptInterface* Game;
    bool term;
    bool IsActiveThreadTerminating() { return term; }
    bool DoesAssassinTrustHero()
    {
        bool banditOutfit = Game->IsWearingClothingItem(Game->GetHero(), "OBJECT_HERO_SHIRT_BANDITCAMP")
            && Game->IsWearingClothingItem(Game->GetHero(), "OBJECT_HERO_TROUSERS_BANDITCAMP")
            && Game->IsWearingClothingItem(Game->GetHero(), "OBJECT_HERO_HAT_BANDITCAMP")
            && Game->IsWearingClothingItem(Game->GetHero(), "OBJECT_HERO_BOOTS_BANDITCAMP")
            && Game->IsWearingClothingItem(Game->GetHero(), "OBJECT_HERO_GLOVES_BANDITCAMP");
        if (banditOutfit) { if (IsActiveThreadTerminating()) return false; return true; }
        bool guardOutfit = Game->IsWearingClothingItem(Game->GetHero(), "OBJECT_HERO_SHIRT_GUARD")
            && Game->IsWearingClothingItem(Game->GetHero(), "OBJECT_HERO_TROUSERS_GUARD")
            && Game->IsWearingClothingItem(Game->GetHero(), "OBJECT_HERO_HAT_GUARD")
            && Game->IsWearingClothingItem(Game->GetHero(), "OBJECT_HERO_BOOTS_GUARD")
            && Game->IsWearingClothingItem(Game->GetHero(), "OBJECT_HERO_GLOVES_GUARD");
        if (guardOutfit) { if (IsActiveThreadTerminating()) return false; return false; }
        if (Game->GetHeroMoralityCategory() < 3) return true;
        return false;
    }
};
static bool Run(const char* outfit, int missing, long morality, bool term)
{
    CGameScriptInterface g; g.outfit = outfit; g.missing = missing; g.morality = morality;
    CV_MurderTwistFinalScript s; s.Game = &g; s.term = term;
    return s.DoesAssassinTrustHero();
}
int main()
{
    bool ok = Run("BANDITCAMP", -1, 5, false) == true
        && Run("BANDITCAMP", -1, 5, true) == false
        && Run("BANDITCAMP", 4, 5, false) == false
        && Run("BANDITCAMP", 2, 1, false) == true
        && Run("GUARD", -1, 0, false) == false
        && Run("GUARD", 0, 0, false) == true
        && Run("GUARD", 3, 3, false) == false;
    if (!ok) { std::printf("00eb03a0_TEST FAIL\n"); return 1; }
    std::printf("00eb03a0_TEST PASS\n");
    return 0;
}