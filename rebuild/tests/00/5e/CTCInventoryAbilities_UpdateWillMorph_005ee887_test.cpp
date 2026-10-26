// Behaviour test for CTCInventoryAbilities::UpdateWillMorph 0x005EE887 (model copy with stub components)
#include <cstdio>

static float g_amount = -1.0f; static int g_setCalls = 0;
class CTCHeroMorph { public: void SetWillMorphAmount(float a) { g_amount = a; ++g_setCalls; } };
class CTCHeroExperience { public:
    long GetTrainableStatMaxLevel(long stat) { return stat == 9 ? 4 : 0; }
    long GetTrainableStatLevel(long stat) { return stat == 9 ? 1 : 0; }
};
class CThing { public:
    CTCHeroMorph* morph; CTCHeroExperience* experience;
    template<class T> bool GetInterface(T** out, long wanted)
    {
        if (wanted == 3 && morph) { *out = (T*)morph; return true; }
        if (wanted == 0x68 && experience) { *out = (T*)experience; return true; }
        return false;
    }
};
struct SAbilityEntry { long Level; void* PDef; };
struct CAbilityEntryVector {
    SAbilityEntry* first; SAbilityEntry* last;
    unsigned long size() const { return (unsigned long)(last - first); }
    SAbilityEntry& operator[](unsigned long n) { return *(first + n); }
};
class CTCInventoryAbilities { public:
    CThing* ParentThing;
    bool Initialised;
    CAbilityEntryVector Abilities;
    bool IsAbilityPotentiallyAvailable(long ability) { return ability != 1; }
    long GetAbilityMaximumLevel(long) { return 2; }
    void UpdateWillMorph();
};

void CTCInventoryAbilities::UpdateWillMorph()
{
    if (!Initialised)
        return;
    CTCHeroMorph* morph;
    if (!ParentThing->GetInterface(&morph, 3))
        return;
    long possible = 0;
    long reached = 0;
    for (unsigned long i = 0; i < Abilities.size(); ++i) {
        if (IsAbilityPotentiallyAvailable(i)) {
            possible += GetAbilityMaximumLevel(i) + 1;
            reached += Abilities[i].Level + 1;
        }
    }
    CTCHeroExperience* experience;
    if (ParentThing->GetInterface(&experience, 0x68)) {
        possible += experience->GetTrainableStatMaxLevel(9);
        reached += experience->GetTrainableStatLevel(9);
    }
    if (possible > 0)
        morph->SetWillMorphAmount((float)reached / possible);
}

int main()
{
    CTCHeroMorph morph; CTCHeroExperience exp;
    CThing hero = { &morph, &exp };
    SAbilityEntry entries[3] = { { 1, 0 }, { 2, 0 }, { -1, 0 } };   // ability 1 not potentially available
    CTCInventoryAbilities a; a.ParentThing = &hero; a.Initialised = true;
    a.Abilities.first = entries; a.Abilities.last = entries + 3;
    a.UpdateWillMorph();   // possible = 3 + 3 + 4 = 10, reached = 2 + 0 + 1 = 3
    if (g_setCalls != 1 || g_amount != 0.3f) { std::printf("005ee887_TEST FAIL\n"); return 1; }
    a.Initialised = false; a.UpdateWillMorph();
    hero.morph = 0; a.Initialised = true; a.UpdateWillMorph();
    if (g_setCalls != 1) { std::printf("005ee887_TEST FAIL\n"); return 1; }
    std::printf("005ee887_TEST PASS\n");
    return 0;
}