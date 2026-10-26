// Behaviour test for 005ee02e: src/005ee02e.cpp copied verbatim; a hero stats def with two tracks so the null
// ability, the already-available ability, an unlockable one and an unknown one are checked.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCInventoryAbilities::IsAbilityPotentiallyAvailable(EHeroAbility) const -- an ability is potentially available when it is already available or some trainable-stat track unlocks it
// Retry 10b: the stats def comes from an inline const getter (`GetDefinitionManager()->GetHeroStatsDef()`, B-4/B-5 accessor idiom): retail keeps the def in ecx and the track-vector pointer in esi; the plain member read swaps them.
// Leaf: IsAbilityPotentiallyAvailable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005EE02E, 185 bytes; frame-pointer size TU
// (`#pragma optimize("y", off)` + `optimize("s", on)`), `ret 4`.
// Callees: CTCBase::GetDefinitionManager() @ 0x006869D0 (+0xDC its CHeroStatsDef).
// Shape: no ability (0) is false; `Abilities[ability].Level >= 0` (the inlined IsAbilityAvailable 0x005EDF08)
// is true; then two index loops -- over CHeroStatsDef::StatLevels (0x50-byte CAbilityTrackDef records) and each
// track's Abilities (0x48-byte CAbilityTrackLevelDef records) -- whose sizes are re-divided with `idiv` every pass
// (size TU), the indices living in the frame and the element addresses strength-reduced (`add esi,50h`, the
// inner pointer spilled to [ebp-0Ch]); a track level unlocking the ability is true, the shared false exit is first.
// Layout: retail CTCInventoryAbilities Abilities vector at +0x160 (PDB +0x180; 8-byte {Level, PDef} entries, as in
// the landed GetAbilityMaximumLevel 0x005EDEA3); CHeroStatsDef::StatLevels +0xFC; CAbilityTrackDef::Abilities +0x40;
// CAbilityTrackLevelDef::AbilityUnlocked +0x40 (PDB +0x44; retail defs sit 4 bytes below the PDB).
#pragma optimize("y", off)
#pragma optimize("s", on)

enum EHeroAbility { HERO_ABILITY_NULL };

class CAbilityTrackLevelDef { public:                       // 0x48 bytes in retail
    unsigned char _base_0x00[0x3C];
    long          ExperienceCost;                    // +0x3C
    EHeroAbility  AbilityUnlocked;                   // +0x40
    long          Description;                       // +0x44
};

template <class T> struct CRetailVector {            // STLport vector {first, last, end}
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    const T& operator[](unsigned long n) const { return _M_start[n]; }
};

class CAbilityTrackDef { public:                            // 0x50 bytes in retail
    unsigned char                         _base_0x00[0x3C];
    long                                  ExperienceStat;   // +0x3C
    CRetailVector<CAbilityTrackLevelDef>  Abilities;        // +0x40
    long                                  MaxedOutDescription;   // +0x4C
};

class CHeroStatsDef { public:
    unsigned char                    _pad_0x00[0xFC];
    CRetailVector<CAbilityTrackDef>  StatLevels;     // +0xFC (PDB +0x114)
};

class CDefinitionManager { public:
    unsigned char  _pad_0x00[0xDC];
    CHeroStatsDef* PHeroStatsDef;                    // +0xDC
    const CHeroStatsDef* GetHeroStatsDef() const { return PHeroStatsDef; }
};

class CHeroAbilityDef;
struct SAbilityEntry {                               // pair<long, CDefPointer<const CHeroAbilityDef>>
    long                   Level;                    // +0x00
    const CHeroAbilityDef* PDef;                     // +0x04
};

class CTCInventoryAbilities { public:
    void*                        __vftable;          // +0x000
    unsigned char                _pad_0x004[0x15C];
    CRetailVector<SAbilityEntry> Abilities;          // +0x160 (PDB +0x180)

    CDefinitionManager* GetDefinitionManager() const;    // 0x006869D0 (CTCBase)
    bool IsAbilityPotentiallyAvailable(EHeroAbility ability) const;
};

bool CTCInventoryAbilities::IsAbilityPotentiallyAvailable(EHeroAbility ability) const
{
    if (ability != 0) {
        if (Abilities[ability].Level >= 0)
            return true;
        const CHeroStatsDef* def = GetDefinitionManager()->GetHeroStatsDef();
        for (unsigned long i = 0; i < def->StatLevels.size(); ++i) {
            for (unsigned long j = 0; j < def->StatLevels[i].Abilities.size(); ++j) {
                if (def->StatLevels[i].Abilities[j].AbilityUnlocked == ability)
                    return true;
            }
        }
    }
    return false;
}
// ---- end source ----
static CDefinitionManager g_dm;
CDefinitionManager* CTCInventoryAbilities::GetDefinitionManager() const { return &g_dm; }
int main()
{
    static CAbilityTrackLevelDef levels0[2], levels1[1];
    memset(levels0, 0, sizeof(levels0)); memset(levels1, 0, sizeof(levels1));
    levels0[0].AbilityUnlocked = (EHeroAbility)3; levels0[1].AbilityUnlocked = (EHeroAbility)5; levels1[0].AbilityUnlocked = (EHeroAbility)9;
    static CAbilityTrackDef tracks[2]; memset(tracks, 0, sizeof(tracks));
    tracks[0].Abilities._M_start = levels0; tracks[0].Abilities._M_finish = levels0 + 2;
    tracks[1].Abilities._M_start = levels1; tracks[1].Abilities._M_finish = levels1 + 1;
    static CHeroStatsDef stats; memset(&stats, 0, sizeof(stats));
    stats.StatLevels._M_start = tracks; stats.StatLevels._M_finish = tracks + 2;
    g_dm.PHeroStatsDef = &stats;
    static SAbilityEntry entries[12]; for (int i = 0; i < 12; ++i) { entries[i].Level = -1; entries[i].PDef = 0; }
    entries[7].Level = 0;
    static unsigned char raw[sizeof(CTCInventoryAbilities)];
    CTCInventoryAbilities* inv = (CTCInventoryAbilities*)raw;
    inv->Abilities._M_start = entries; inv->Abilities._M_finish = entries + 12;
    CHECK(inv->IsAbilityPotentiallyAvailable((EHeroAbility)0) == false);
    CHECK(inv->IsAbilityPotentiallyAvailable((EHeroAbility)7) == true);    // already available
    CHECK(inv->IsAbilityPotentiallyAvailable((EHeroAbility)5) == true);    // track 0 level 1
    CHECK(inv->IsAbilityPotentiallyAvailable((EHeroAbility)9) == true);    // track 1
    CHECK(inv->IsAbilityPotentiallyAvailable((EHeroAbility)4) == false);
    if (fails == 0) printf("005ee02e_TEST PASS\n"); else printf("005ee02e_TEST FAIL\n");
    return fails;
}