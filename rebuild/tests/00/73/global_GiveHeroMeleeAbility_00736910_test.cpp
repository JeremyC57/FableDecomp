// Behaviour test for (anonymous)::GiveHeroMeleeAbility (0x00736910): on a live hero with a combat component, add or remove the flourish ability.
#include <cstdio>
class CTCCombat { public: int Added, Removed, Last;
    void AddMeleeFlourishAbility(int d) { ++Added; Last = d; }
    void RemoveMeleeFlourishAbility(int d) { ++Removed; Last = d; } };
class CThing { public: bool Dead; bool HasCombat; CTCCombat* Combat; };
class CPlayer { public: CThing* Creature; CThing* GetPControlledCreature() const { return Creature; } };
class CPlayerManager { public: CPlayer Main; CPlayer& GetMainPlayer() { return Main; } };
class CMainGameComponent { public: CPlayerManager* PPlayerManager; };
static CMainGameComponent* g_pMainGameComponent;
static void GiveHeroMeleeAbility(int defIndex, bool give)
{
    CThing* hero = g_pMainGameComponent->PPlayerManager->GetMainPlayer().GetPControlledCreature();
    if (hero && !hero->Dead) {
        CTCCombat* combat = hero->HasCombat ? hero->Combat : 0;
        if (give) combat->AddMeleeFlourishAbility(defIndex);
        else combat->RemoveMeleeFlourishAbility(defIndex);
    }
}
int main()
{
    CTCCombat c; c.Added = c.Removed = 0; c.Last = 0;
    CThing hero; hero.Dead = false; hero.HasCombat = true; hero.Combat = &c;
    CPlayerManager pm; pm.Main.Creature = &hero;
    CMainGameComponent mgc; mgc.PPlayerManager = &pm; g_pMainGameComponent = &mgc;
    GiveHeroMeleeAbility(12, true);
    GiveHeroMeleeAbility(13, false);
    if (c.Added != 1 || c.Removed != 1 || c.Last != 13) { std::printf("00736910_TEST FAIL\n"); return 1; }
    hero.Dead = true; GiveHeroMeleeAbility(14, true);
    pm.Main.Creature = 0; GiveHeroMeleeAbility(15, true);
    if (c.Added != 1 || c.Last != 13) { std::printf("00736910_TEST FAIL\n"); return 1; }
    std::printf("00736910_TEST PASS\n");
    return 0;
}