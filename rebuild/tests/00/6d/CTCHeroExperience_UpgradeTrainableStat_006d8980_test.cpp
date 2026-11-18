#include <stdio.h>
enum EHeroTrainableStatType { FIRST, SECOND };
struct CTCHeroStats {
    float age;
    float GetAge() const { return age; }
    void SetAge(float value) { age=value; }
};
struct CGlobalDef { float AgeIncreasePerLevelUp; };
struct CTCHeroExperience {
    long TrainableStatLevels[2], trackCounts[2];
    int calls;
    bool present;
    CTCHeroStats stats;
    CGlobalDef global;
    long GetTrainableStatMaxLevel(EHeroTrainableStatType t) const {
        return trackCounts[t] ? trackCounts[t]-1 : 0;
    }
    void UpdateAbilitiesForStat(EHeroTrainableStatType t,bool silent) {
        calls=(t==SECOND && !silent && TrainableStatLevels[t]==2) ? 1 : -100;
    }
    void OnStatLevelChange(bool upgraded) { calls=calls*10+(upgraded ? 2 : -100); }
    CTCHeroExperience& GetDefinitionManager() { return *this; }
    CTCHeroExperience& GetParentThing() { return *this; }
    const CGlobalDef& PeekGlobalDef() { return global; }
    bool GetTC(CTCHeroStats** out) { if (present) *out=&stats; return present; }
    void UpgradeTrainableStat(EHeroTrainableStatType);
};
void CTCHeroExperience::UpgradeTrainableStat(EHeroTrainableStatType stat_type)
{
    if (TrainableStatLevels[stat_type] < GetTrainableStatMaxLevel(stat_type)) {
        TrainableStatLevels[stat_type] += 1;
        UpdateAbilitiesForStat(stat_type, false);
        OnStatLevelChange(true);
        const CGlobalDef& global_def = GetDefinitionManager().PeekGlobalDef();
        CTCHeroStats* ptc_hero_stats;
        if (GetParentThing().GetTC(&ptc_hero_stats))
            ptc_hero_stats->SetAge(ptc_hero_stats->GetAge() + global_def.AgeIncreasePerLevelUp);
    }
}
int main() {
    for (int n=0;n<=4;++n) for (int present=0;present!=2;++present) {
        CTCHeroExperience hero;
        hero.trackCounts[FIRST]=8; hero.TrainableStatLevels[FIRST]=4;
        hero.trackCounts[SECOND]=n; hero.TrainableStatLevels[SECOND]=1;
        hero.global.AgeIncreasePerLevelUp=0.5f; hero.stats.age=18.25f;
        hero.calls=0; hero.present=present!=0;
        hero.UpgradeTrainableStat(SECOND);
        if (hero.TrainableStatLevels[FIRST]!=4 || hero.TrainableStatLevels[SECOND]!=(n>2 ? 2 : 1)) return 1;
        if (hero.calls!=(n>2 ? 12 : 0) || hero.stats.age!=(n>2 && present ? 18.75f : 18.25f)) return 2;
    }
    puts("006d8980_TEST PASS"); return 0;
}