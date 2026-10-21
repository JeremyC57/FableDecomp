// Behaviour test for 006d6660: src/006d6660.cpp copied verbatim (shared headers expanded inline), callees stubbed below so calls
// and arguments are observable. CTCHeroExperience::GetTotalStatLevel(EHeroExperienceStatCategory category) const
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCHeroExperience::GetTotalStatLevel(EHeroExperienceStatCategory category) const -- sum the trainable stat levels of every stat whose ability track belongs to the category
// Shape: the def is read through PHeroStatsDef-> at each use (no statsDef local): VC7.1 then loads TrainableStatLevels'
// begin into ebx at entry (ebx pushed in the prologue, reused as the 0x50 stride offset) as retail; with a named def
// pointer it takes edx and shrink-wraps the ebx save into the loop branch.
// Leaf: GetTotalStatLevel
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006D6660, length 115 bytes (+13 int3 padding), `ret 4`.
// Shape: `for (i = 0; i < TrainableStatLevels.size() && i < tracks.size(); ++i)`
// (the separate `if (i >= tracks.size()) break;` form duplicates the epilogue); STLport `*(begin() + i)` element access (0x50-byte tracks,
// size by signed division); the stats def is read once through the CDefPointer before the loop.
// Layout: local model (engine/CTCHeroExperience.h is the PDB layout with 16-byte vectors); retail TrainableStatLevels +0x24,
// PHeroStatsDef +0x5c; CHeroStatsDef::StatLevels +0xfc (vector<CAbilityTrackDef>, 0x50 bytes each, ExperienceStat +0x3c,
// Abilities +0x40).
enum EHeroExperienceStatCategory { HERO_STAT_CATEGORY_NONE };
struct CAbilityTrackLevelDef { unsigned char _body[0x48]; };
template <class T> struct CRetailVector {       // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    const T* begin() const { return _M_start; }
    const T* end() const { return _M_finish; }
    unsigned int size() const { return (unsigned int)(end() - begin()); }
    const T& operator[](unsigned int n) const { return *(begin() + n); }
};
struct CAbilityTrackDef {                       // 0x50 bytes in retail
    unsigned char _base00[0x3c];
    EHeroExperienceStatCategory ExperienceStat;            // +0x3c
    CRetailVector<CAbilityTrackLevelDef> Abilities;         // +0x40
    long MaxedOutDescription;                               // +0x4c
};
struct CHeroStatsDef {
    unsigned char _pad00[0xfc];
    CRetailVector<CAbilityTrackDef> StatLevels;             // +0xfc
};
template <class T> struct CDefPointer {
    T* Object;
    const T* operator->() const { return Object; }
};
class CThing;
class CTCHeroExperience {
public:
    virtual ~CTCHeroExperience();
    CThing*             ParentThing;            // +0x04
    unsigned char       _base08[0x4];
    long                FishingLevel;           // +0x0c
    long                TotalExperiencePoints;  // +0x10
    long                ExperiencePointsAvailableToSpend;   // +0x14
    CRetailVector<long> StatExperiencePoints;   // +0x18
    CRetailVector<long> TrainableStatLevels;    // +0x24
    float               ExperienceMultiplierParam;          // +0x30
    float               LastExperienceMultiplierParam;      // +0x34
    long                ExperienceAttenuationTimer;         // +0x38
    bool                AbleToGainExperience;               // +0x3c
    void*               Pickups;                // +0x40
    CRetailVector<long> ExperienceSpentOnStats; // +0x44
    void*               AttackHistory;          // +0x50
    CDefPointer<void>   PDef;                   // +0x54
    CDefPointer<void>   PHeroCombatDef;         // +0x58
    CDefPointer<CHeroStatsDef> PHeroStatsDef;   // +0x5c
    long GetTotalStatLevel(EHeroExperienceStatCategory category) const;
};

long CTCHeroExperience::GetTotalStatLevel(EHeroExperienceStatCategory category) const
{
    long total = 0;
    for (unsigned int i = 0; i < TrainableStatLevels.size() && i < PHeroStatsDef->StatLevels.size(); ++i) {
        if (PHeroStatsDef->StatLevels[i].ExperienceStat == category)
            total += TrainableStatLevels[i];
    }
    return total;
}

// ---- stubs and driver ----
CTCHeroExperience::~CTCHeroExperience() { }
int main()
{
    static CAbilityTrackDef tracks[3];
    memset(tracks, 0, sizeof(tracks));
    tracks[0].ExperienceStat = (EHeroExperienceStatCategory)1;
    tracks[1].ExperienceStat = (EHeroExperienceStatCategory)2;
    tracks[2].ExperienceStat = (EHeroExperienceStatCategory)1;
    CHeroStatsDef def; memset(&def, 0, sizeof(def));
    def.StatLevels._M_start = tracks; def.StatLevels._M_finish = tracks + 3; def.StatLevels._M_end_of_storage = tracks + 3;
    long levels[4] = { 10, 20, 30, 40 };
    CTCHeroExperience h;
    h.TrainableStatLevels._M_start = levels; h.TrainableStatLevels._M_finish = levels + 4; h.TrainableStatLevels._M_end_of_storage = levels + 4;
    h.PHeroStatsDef.Object = &def;
    CHECK(h.GetTotalStatLevel((EHeroExperienceStatCategory)1) == 40);   // tracks 0 and 2 (the 4th level has no track)
    CHECK(h.GetTotalStatLevel((EHeroExperienceStatCategory)2) == 20);
    CHECK(h.GetTotalStatLevel((EHeroExperienceStatCategory)3) == 0);
    h.TrainableStatLevels._M_finish = levels + 1;                       // fewer levels than tracks
    CHECK(h.GetTotalStatLevel((EHeroExperienceStatCategory)1) == 10);
    h.TrainableStatLevels._M_finish = levels;                           // empty
    CHECK(h.GetTotalStatLevel((EHeroExperienceStatCategory)1) == 0);
    if (fails == 0) printf("006d6660_TEST PASS\n"); else printf("006d6660_TEST FAIL\n");
    return fails;
}
