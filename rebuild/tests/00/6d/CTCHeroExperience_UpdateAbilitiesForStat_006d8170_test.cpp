#include <cstdio>
// behaviour test: UpdateAbilitiesForStat sets max health (stat 1) or max stamina (stat 9) from the per-level table clamped to
// the table's last entry plus the hero stats' extra amount, and heals/refills by the non-negative gain unless silent
class CTCHeroStats { public:
    long MaxStamina; float ExtraMaxHealth; long ExtraMaxStamina; long modified;
    void SetMaxStamina(long m) { MaxStamina = m; }
    void ModifyStaminaBy(long a) { modified += a; }
};
class CThingPhysical { public:
    CTCHeroStats* stats; float MaxHealth; float healed;
    bool GetInterface(const CTCHeroStats** out, long type) const { if (type == 4 && stats) { *out = stats; return true; } return false; }
    unsigned long HasTC(long type) const { return type == 4 && stats ? 1u : 0u; }
    CTCHeroStats* GetTC(long) { return stats; }
    void SetMaxHealth(float h, bool) { MaxHealth = h; }
    void ModifyHealth(float a, bool) { healed += a; }
};
template <class T> inline const T& Min(const T& a, const T& b) { return b < a ? b : a; }
class CTCHeroExperience { public:
    CThingPhysical* ParentThing; long TrainableStatLevels[10]; float HP[3]; long MP[4];
    void UpdateAbilitiesForStat(int stat, bool silent)
    {
        if (stat == 1) {
            CThingPhysical* creature = ParentThing;
            float extra = 0.0f;
            const CThingPhysical* thing = creature;
            const CTCHeroStats* heroStats;
            if (thing->GetInterface(&heroStats, 4))
                extra = heroStats->ExtraMaxHealth;
            const long& level = Min(TrainableStatLevels[1], (long)3 - 1);
            float maxHealth = HP[level] + extra;
            float increase = maxHealth - creature->MaxHealth;
            if (increase < 0.0f) increase = 0.0f;
            creature->SetMaxHealth(maxHealth, false);
            if (!silent) creature->ModifyHealth(increase, false);
        } else if (stat == 9) {
            CThingPhysical* thing = ParentThing;
            if (thing->HasTC(4)) {
                CTCHeroStats* stats = thing->GetTC(4);
                long extra = stats->ExtraMaxStamina;
                const long& level = Min(TrainableStatLevels[9], (long)4 - 1);
                long maxStamina = MP[level] + extra;
                long increase = maxStamina - stats->MaxStamina;
                if (increase < 0) increase = 0;
                stats->SetMaxStamina(maxStamina);
                if (!silent) stats->ModifyStaminaBy(increase);
            }
        }
    }
};
int main()
{
    CTCHeroStats st = { 50, 5.0f, 7, 0 };
    CThingPhysical c = { &st, 100.0f, 0.0f };
    CTCHeroExperience e = { &c, { 0, 5, 0, 0, 0, 0, 0, 0, 0, 1 }, { 100.0f, 150.0f, 200.0f }, { 40, 60, 80, 100 } };
    e.UpdateAbilitiesForStat(1, false);         // level 5 clamps to 2 -> 200 + 5 = 205, heal 105
    bool ok = c.MaxHealth == 205.0f && c.healed == 105.0f;
    e.TrainableStatLevels[1] = 0; e.UpdateAbilitiesForStat(1, false);   // 100 + 5 = 105: negative gain clamps to 0
    ok = ok && c.MaxHealth == 105.0f && c.healed == 105.0f;
    e.UpdateAbilitiesForStat(9, true);          // silent: 60 + 7 = 67, no refill
    ok = ok && st.MaxStamina == 67 && st.modified == 0;
    e.TrainableStatLevels[9] = 8; e.UpdateAbilitiesForStat(9, false);   // clamps to 3 -> 107, refill 40
    ok = ok && st.MaxStamina == 107 && st.modified == 40;
    c.stats = 0; e.UpdateAbilitiesForStat(1, true);   // no hero stats: extra 0
    ok = ok && c.MaxHealth == 100.0f;
    e.UpdateAbilitiesForStat(3, false);         // other stats: nothing
    ok = ok && c.MaxHealth == 100.0f && st.MaxStamina == 107;
    if (!ok) { std::printf("006d8170_TEST FAIL\n"); return 1; }
    std::printf("006d8170_TEST PASS\n");
    return 0;
}