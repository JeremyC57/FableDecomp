// Behaviour test for CTCInventoryExperience::BuyAbility (0x005785B3): snapshot first, find-or-insert the ability's undo
// record (a fresh record only when missing), SaveBefore, raise the ability level by one, spend the cost in the ability's
// stat category, SaveAfter, count the bought level and flag the screen rebuild.
#include <cstdio>

enum EHeroAbility { ABILITY_A = 3, ABILITY_B = 9 };
enum EHeroExperienceStatCategory { CAT_STRENGTH = 1, CAT_WILL = 3 };

static int g_log[32];
static int g_logCount = 0;
static void Log(int v) { g_log[g_logCount++] = v; }

class CTCHeroExperience {
public:
    long Spent[4];
    long Pool;
    void SpendExperience(EHeroExperienceStatCategory category, long amount) { Spent[category] += amount; Pool -= amount; Log(30); }
};
class CTCInventoryAbilities {
public:
    long Levels[16];
    long GetAbilityLevel(EHeroAbility ability) const { return Levels[ability]; }
    void SetAbilityLevelTo(EHeroAbility ability, long level) { Levels[ability] = level; Log(20); }
    EHeroExperienceStatCategory GetAbilityStatType(EHeroAbility ability) const { return ability == ABILITY_A ? CAT_STRENGTH : CAT_WILL; }
};
struct SExpSpendUndo {
    long Levels;
    long Before, After;
    SExpSpendUndo() : Levels(0), Before(-1), After(-1) { }
    void SaveBefore(CTCHeroExperience* e) { Before = e->Pool; Log(10); }
    void SaveAfter(CTCHeroExperience* e) { After = e->Pool; Log(40); }
};
struct SUndoEntry { bool used; EHeroAbility key; SExpSpendUndo value; };
struct CUndoMap {
    SUndoEntry entries[4];
    int inserts;
    SUndoEntry* end() { return 0; }
    SUndoEntry* find(EHeroAbility key) { for (int i = 0; i < 4; ++i) if (entries[i].used && entries[i].key == key) return &entries[i]; return 0; }
    void insert(EHeroAbility key, const SExpSpendUndo& v)
    {
        ++inserts;
        for (int i = 0; i < 4; ++i) if (!entries[i].used) { entries[i].used = true; entries[i].key = key; entries[i].value = v; return; }
    }
};

class CTCInventoryExperience {
public:
    CUndoMap UndoableAbilityBuys;
    bool DestroyEverything;
    CTCHeroExperience Hero;
    CTCInventoryAbilities Abilities;
    int Snapshots;
    void SaveExperienceValuesForCountDown() { ++Snapshots; Log(1); }
    CTCHeroExperience* GetTCHeroExperience() { return &Hero; }
    CTCInventoryAbilities* GetTCInventoryAbilities() { return &Abilities; }
    void BuyAbility(EHeroAbility ability, long cost)
    {
        SaveExperienceValuesForCountDown();
        CUndoMap& undos = UndoableAbilityBuys;
        SUndoEntry* it = undos.find(ability);
        if (it == undos.end()) {
            SExpSpendUndo undo;
            undos.insert(ability, undo);
            it = undos.find(ability);
        }
        it->value.SaveBefore(GetTCHeroExperience());
        GetTCInventoryAbilities()->SetAbilityLevelTo(ability, GetTCInventoryAbilities()->GetAbilityLevel(ability) + 1);
        GetTCHeroExperience()->SpendExperience(GetTCInventoryAbilities()->GetAbilityStatType(ability), cost);
        it->value.SaveAfter(GetTCHeroExperience());
        ++it->value.Levels;
        DestroyEverything = true;
    }
};

int main()
{
    static CTCInventoryExperience inv;
    inv.Hero.Pool = 1000;
    inv.Abilities.Levels[ABILITY_A] = 2;
    inv.BuyAbility(ABILITY_A, 100);
    SUndoEntry* e = inv.UndoableAbilityBuys.find(ABILITY_A);
    if (!e || e->value.Levels != 1 || e->value.Before != 1000 || e->value.After != 900) { std::printf("005785b3_TEST FAIL\n"); return 1; }
    if (inv.Abilities.Levels[ABILITY_A] != 3 || inv.Hero.Spent[CAT_STRENGTH] != 100 || !inv.DestroyEverything) { std::printf("005785b3_TEST FAIL\n"); return 1; }
    int expect[5] = { 1, 10, 20, 30, 40 };
    if (g_logCount != 5) { std::printf("005785b3_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 5; ++i) if (g_log[i] != expect[i]) { std::printf("005785b3_TEST FAIL\n"); return 1; }
    inv.DestroyEverything = false;
    inv.BuyAbility(ABILITY_A, 50);   // existing record: no second insert, Levels counts 2, Before is overwritten
    if (inv.UndoableAbilityBuys.inserts != 1 || e->value.Levels != 2 || e->value.Before != 900 || e->value.After != 850) { std::printf("005785b3_TEST FAIL\n"); return 1; }
    inv.BuyAbility(ABILITY_B, 10);
    if (inv.UndoableAbilityBuys.inserts != 2 || inv.Hero.Spent[CAT_WILL] != 10 || inv.Abilities.Levels[ABILITY_B] != 1 || inv.Snapshots != 3) { std::printf("005785b3_TEST FAIL\n"); return 1; }
    std::printf("005785b3_TEST PASS\n");
    return 0;
}