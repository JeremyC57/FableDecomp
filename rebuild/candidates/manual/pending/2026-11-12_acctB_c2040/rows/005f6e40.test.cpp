// Behaviour test for 0x005F6E40 CTCInventoryAbilities::OnSerialise(CPersistContext&, bool): a host model of the row's logic
// (same statement order as src/005f6e40.cpp) over plain arrays, a recording persist context and a name -> ability def table.
// Save: the levelled abilities' def names and levels, one name per special-move button ("" for an empty or unknown button)
// and one entry per received ability ("" for a negative level, nothing for a levelled slot without a def). Load: the names
// are looked up again; abilities are made available with their saved levels (Initialised cleared meanwhile), buttons are
// re-assigned, the received order is rebuilt ("" -> HERO_ABILITY_NULL). Both paths then rebuild the assigned palettes from the
// received order (at most TotalSpellsInPalettes entries, padded with HERO_ABILITY_NULL) unless the world is only about to
// load the hero state, and finally call the base OnSerialise.
#include <cstdio>
#include <cstring>

enum { MAX_ABILITIES = 20, MAX_LIST = 64, HERO_ABILITY_NULL = 0 };

struct NameList {                                   // vector<CCharString>
    const char* items[MAX_LIST];
    unsigned long count;
    NameList() : count(0) {}
    void push_back(const char* s) { items[count++] = s; }
    void clear() { count = 0; }
    unsigned long size() const { return count; }
};
struct LongList {                                   // vector<long> / vector<EHeroAbility>
    long items[MAX_LIST];
    unsigned long count;
    LongList() : count(0) {}
    void push_back(long v) { items[count++] = v; }
    void clear() { count = 0; }
    void resize(unsigned long n, long v) { while (count < n) items[count++] = v; count = n; }
    unsigned long size() const { return count; }
};

struct AbilityDef { const char* name; long ability; };
static const AbilityDef g_defs[] = {
    { "HERO_ABILITY_FORCE_PUSH", 1 }, { "HERO_ABILITY_TIME_SPELL", 2 }, { "HERO_ABILITY_ENFLAME_SPELL", 3 },
    { "HERO_ABILITY_BERSERK", 8 }, { "HERO_ABILITY_LIGHTNING_SPELL", 11 },
};
static const AbilityDef* Lookup(const char* name)     // GetPDefFromInstantiationName
{
    for (unsigned i = 0; i < sizeof(g_defs) / sizeof(g_defs[0]); ++i)
        if (std::strcmp(g_defs[i].name, name) == 0) return &g_defs[i];
    return 0;
}

struct Context {                                    // CPersistContext with a by-name store
    bool saving;
    NameList heroAbility, abilityButton, receivedAbilities;
    LongList abilityLevels;
    void TransferVector(const char* name, NameList& v)
    {
        NameList* slot = std::strcmp(name, "HeroAbility") == 0 ? &heroAbility
                       : std::strcmp(name, "AbilityButton") == 0 ? &abilityButton : &receivedAbilities;
        if (saving) *slot = v; else v = *slot;
    }
    void TransferVector(const char*, LongList& v) { if (saving) abilityLevels = v; else v = abilityLevels; }
};

struct Abilities {
    long level[MAX_ABILITIES];
    const AbilityDef* def[MAX_ABILITIES];
    long buttonKey[8], buttonAbility[8]; long buttonCount;   // CVectorMap<long, EHeroAbility> AbilityButtons (sorted keys)
    long assignedButton[8];
    LongList receivedOrder, assigned, assignedSave;
    bool initialised, aboutToLoadHeroState;
    long numButtons, totalSpellsInPalettes, available[MAX_ABILITIES], baseCalls, initialiseCalls;
    Abilities() : buttonCount(0), initialised(false), aboutToLoadHeroState(false), numButtons(4), totalSpellsInPalettes(6),
                  baseCalls(0), initialiseCalls(0)
    {
        for (int i = 0; i < MAX_ABILITIES; ++i) { level[i] = -1; def[i] = 0; available[i] = 0; }
        for (int b = 0; b < 8; ++b) assignedButton[b] = -1;
    }
    long Find(long key) const { for (long i = 0; i < buttonCount; ++i) if (buttonKey[i] == key) return i; return -1; }
    void InitialiseAbilities()
    {
        ++initialiseCalls;
        for (unsigned i = 0; i < sizeof(g_defs) / sizeof(g_defs[0]); ++i) def[g_defs[i].ability] = &g_defs[i];
    }
    void SetAbilityAsAvailable(long ability, bool, bool) { available[ability] = initialised ? 2 : 1; if (level[ability] < 0) level[ability] = 0; }
    void SetAbilityToButton(long ability, long button, bool) { assignedButton[button] = ability; }

    void OnSerialise(Context& context)
    {
        if (context.saving) {
            NameList ability_names; LongList ability_levels;
            for (long ability = 0; ability < MAX_ABILITIES; ++ability)
                if (level[ability] >= 0 && def[ability]) { ability_names.push_back(def[ability]->name); ability_levels.push_back(level[ability]); }
            context.TransferVector("HeroAbility", ability_names);
            context.TransferVector("AbilityLevels", ability_levels);
            ability_names.clear();
            for (long button = 0; button < numButtons; ++button) {
                long it = Find(button);
                if (it >= 0) {
                    if (def[buttonAbility[it]]) ability_names.push_back(def[buttonAbility[it]]->name);
                    else ability_names.push_back("");
                } else ability_names.push_back("");
            }
            context.TransferVector("AbilityButton", ability_names);
            NameList received_names;
            for (unsigned long i = 0; i < receivedOrder.size(); ++i) {
                long ability = receivedOrder.items[i];
                if (level[ability] >= 0) { if (def[ability]) received_names.push_back(def[ability]->name); }
                else received_names.push_back("");
            }
            context.TransferVector("ReceivedAbilities", received_names);
        } else {
            NameList ability_names;
            if (!aboutToLoadHeroState) { InitialiseAbilities(); initialised = true; }
            context.TransferVector("HeroAbility", ability_names);
            LongList ability_levels;
            context.TransferVector("AbilityLevels", ability_levels);
            if (!aboutToLoadHeroState) {
                bool was = initialised; initialised = false;
                for (unsigned long i = 0; i < ability_names.size(); ++i) {
                    const AbilityDef* d = Lookup(ability_names.items[i]);
                    if (d) { SetAbilityAsAvailable(d->ability, true, false); if (i < ability_levels.size()) level[d->ability] = ability_levels.items[i]; }
                }
                initialised = was;
            }
            ability_names.clear();
            context.TransferVector("AbilityButton", ability_names);
            long num_buttons = numButtons;
            for (long button = 0; button < num_buttons; ++button) {
                const AbilityDef* d = Lookup(ability_names.items[button]);
                if (d) SetAbilityToButton(d->ability, button, true);
            }
            if (!aboutToLoadHeroState) { receivedOrder.clear(); receivedOrder.resize(totalSpellsInPalettes, HERO_ABILITY_NULL); }
            NameList received_names;
            context.TransferVector("ReceivedAbilities", received_names);
            if (!aboutToLoadHeroState) {
                for (unsigned long i = 0; i < received_names.size(); ++i) {
                    const char* name = received_names.items[i];
                    if (std::strcmp(name, "") == 0) receivedOrder.items[i] = HERO_ABILITY_NULL;
                    else { const AbilityDef* d = Lookup(name); if (d) receivedOrder.items[i] = d->ability; }
                }
            }
        }
        if (!aboutToLoadHeroState) {
            assigned.clear(); assignedSave.clear();
            unsigned long count;
            if (receivedOrder.size() <= (unsigned long)totalSpellsInPalettes) count = receivedOrder.size();
            else count = totalSpellsInPalettes;
            for (unsigned long i = 0; i < count; ++i) { assigned.push_back(receivedOrder.items[i]); assignedSave.push_back(receivedOrder.items[i]); }
            unsigned long n = assigned.size();
            if (n < (unsigned long)totalSpellsInPalettes)
                for (unsigned long j = 0; j < totalSpellsInPalettes - n; ++j) { assigned.push_back(HERO_ABILITY_NULL); assignedSave.push_back(HERO_ABILITY_NULL); }
        }
        ++baseCalls;
    }
};

static int fails = 0;
static void check(bool c, int line) { if (!c) { std::printf("check failed at line %d\n", line); ++fails; } }
#define CHECK(c) check((c), __LINE__)

int main()
{
    // --- save
    Abilities a;
    a.def[1] = &g_defs[0]; a.level[1] = 2;                      // force push, level 2
    a.def[3] = &g_defs[2]; a.level[3] = 0;                      // enflame, level 0
    a.def[8] = &g_defs[3]; a.level[8] = -1;                     // berserk not levelled
    a.level[5] = 1;                                             // levelled without a def: skipped
    a.buttonKey[0] = 0; a.buttonAbility[0] = 3;
    a.buttonKey[1] = 2; a.buttonAbility[1] = 5;                 // button 2 -> ability without def -> ""
    a.buttonCount = 2;
    a.receivedOrder.push_back(3); a.receivedOrder.push_back(8); a.receivedOrder.push_back(5); a.receivedOrder.push_back(1);
    Context c; c.saving = true;
    a.OnSerialise(c);
    CHECK(c.heroAbility.size() == 2 && std::strcmp(c.heroAbility.items[0], "HERO_ABILITY_FORCE_PUSH") == 0
          && std::strcmp(c.heroAbility.items[1], "HERO_ABILITY_ENFLAME_SPELL") == 0);
    CHECK(c.abilityLevels.size() == 2 && c.abilityLevels.items[0] == 2 && c.abilityLevels.items[1] == 0);
    CHECK(c.abilityButton.size() == 4 && std::strcmp(c.abilityButton.items[0], "HERO_ABILITY_ENFLAME_SPELL") == 0
          && std::strcmp(c.abilityButton.items[1], "") == 0 && std::strcmp(c.abilityButton.items[2], "") == 0
          && std::strcmp(c.abilityButton.items[3], "") == 0);
    // received: 3 -> name, 8 (level -1) -> "", 5 (level 1, no def) -> nothing, 1 -> name
    CHECK(c.receivedAbilities.size() == 3 && std::strcmp(c.receivedAbilities.items[0], "HERO_ABILITY_ENFLAME_SPELL") == 0
          && std::strcmp(c.receivedAbilities.items[1], "") == 0
          && std::strcmp(c.receivedAbilities.items[2], "HERO_ABILITY_FORCE_PUSH") == 0);
    CHECK(a.assigned.size() == 6 && a.assignedSave.size() == 6 && a.assigned.items[0] == 3 && a.assigned.items[3] == 1
          && a.assigned.items[4] == HERO_ABILITY_NULL && a.assigned.items[5] == HERO_ABILITY_NULL && a.baseCalls == 1);

    // --- load into a fresh component
    Abilities b;
    c.saving = false;
    c.abilityButton.items[1] = "HERO_ABILITY_LIGHTNING_SPELL";
    b.OnSerialise(c);
    CHECK(b.initialiseCalls == 1 && b.initialised);
    CHECK(b.available[1] == 1 && b.available[3] == 1 && b.level[1] == 2 && b.level[3] == 0);   // made available with Initialised cleared
    CHECK(b.assignedButton[0] == 3 && b.assignedButton[1] == 11 && b.assignedButton[2] == -1 && b.assignedButton[3] == -1);
    CHECK(b.receivedOrder.size() == 6 && b.receivedOrder.items[0] == 3 && b.receivedOrder.items[1] == HERO_ABILITY_NULL
          && b.receivedOrder.items[2] == 1 && b.receivedOrder.items[3] == HERO_ABILITY_NULL);
    CHECK(b.assigned.size() == 6 && b.assigned.items[0] == 3 && b.assigned.items[2] == 1 && b.assignedSave.items[5] == HERO_ABILITY_NULL);

    // --- palette clamp: more received abilities than palette slots
    Abilities d; d.totalSpellsInPalettes = 2; d.aboutToLoadHeroState = false;
    for (long i = 1; i <= 4; ++i) d.receivedOrder.push_back(i);
    Context e; e.saving = true;
    d.OnSerialise(e);
    CHECK(d.assigned.size() == 2 && d.assigned.items[1] == 2 && d.assignedSave.size() == 2);

    // --- about to load the hero state: no initialise, no lookups applied, palettes untouched
    Abilities f; f.aboutToLoadHeroState = true; f.assigned.push_back(7);
    c.saving = false;
    f.OnSerialise(c);
    CHECK(f.initialiseCalls == 0 && f.available[1] == 0 && f.receivedOrder.size() == 0 && f.assigned.size() == 1 && f.baseCalls == 1);
    CHECK(f.assignedButton[0] == 3);                                                            // buttons are always re-assigned

    if (fails) { std::printf("005f6e40_TEST FAIL\n"); return 1; }
    std::printf("005f6e40_TEST PASS\n");
    return 0;
}
