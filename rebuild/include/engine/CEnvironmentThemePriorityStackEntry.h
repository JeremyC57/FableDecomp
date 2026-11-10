#ifndef FABLE_CENVIRONMENTTHEMEPRIORITYSTACKENTRY_H
#define FABLE_CENVIRONMENTTHEMEPRIORITYSTACKENTRY_H
#include "engine/CEnvironmentThemeResourcePallette.h"
namespace NEnvironmentInternal {
class CEnvironmentThemePriorityStackEntry {
    struct CThemeWeight {
        long ThemeID;
        float Weight;
        bool LockWeight;
    };
    float BaseWeight;
    bool BaseWeightLocked;
    CArray<CThemeWeight> Themes;
    long ThemeTransitionTargetID;
    float ThemeTransitionInitialWeight;
    bool ThemeTransitionActive;
    double ThemeTransitionStartTime;
    float ThemeTransitionDuration;
public:
    void SetTheme(long);
    void RestoreDefaultTheme();
    void SetThemeWeight(long,float,bool);
    void SetDefaultThemeWeight(float,bool);
    void StartThemeTransition(long,float);
    void TransitionToDefaultTheme(float);
    float GetLevelThemeWeight() const;
    void BlendInLevel(CBlendedEnvironmentTheme&,const CBlendedEnvironmentTheme&,float,const CEnvironmentThemeResourcePallette&) const;
    void ProcessTransitions();
    void RebalanceWeights();
    float GetThemeWeight(long);
    void GetLevelTheme(CBlendedEnvironmentTheme&,float,const CEnvironmentThemeResourcePallette&);
};
}
FABLE_STATIC_ASSERT(sizeof(NEnvironmentInternal::CEnvironmentThemePriorityStackEntry)==48);
#endif
