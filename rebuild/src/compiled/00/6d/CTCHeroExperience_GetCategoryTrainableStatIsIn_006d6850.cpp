// CTCHeroExperience::GetCategoryTrainableStatIsIn(EHeroTrainableStatType) const @ 006d6850
// Leaf: ?GetCategoryTrainableStatIsIn@CTCHeroExperience@@QBE?AW4EHeroExperienceStatCategory@@W4EHeroTrainableStatType@@@Z
// Identity: Ego_r.pdb / FableWin.pdb const (QBE); landing 2026-11-03f respell (was QAE).

enum EHeroTrainableStatType { };
enum EHeroExperienceStatCategory { };

struct StatEntry {
    char pad0[0x3c];        // 0x00..0x3b
    int  category;          // 0x3c
    char pad1[80 - 0x40];   // pad to 80 bytes total
};

struct StatTable {
    char pad0[0xfc];        // 0x00..0xfb
    StatEntry* entries;     // 0xfc
};

class CTCHeroExperience { public:
    char pad0[0x5c];        // 0x00..0x5b
    StatTable* table;       // 0x5c
    EHeroExperienceStatCategory GetCategoryTrainableStatIsIn(EHeroTrainableStatType stat) const;
};

EHeroExperienceStatCategory CTCHeroExperience::GetCategoryTrainableStatIsIn(EHeroTrainableStatType stat) const
{
    return (EHeroExperienceStatCategory)this->table->entries[(int)stat].category;
}