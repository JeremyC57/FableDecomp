// NEnvironmentInternal::CEnvironmentThemePriorityStackEntry::ProcessTransitions() -- advance active theme weights from elapsed time.
// Retail 00887a30, 203 bytes. GFGetTime 009e1bc0, SetOnlyTheme 00887330,
// RestoreDefaultTheme 00887380, SetThemeWeight 00886d70, RebalanceWeights 00886200.
// PDB names retained; no generated class header. Native CArray is 12 bytes rather than donor16.
double __cdecl GFGetTime();
namespace NEnvironmentInternal {
struct CEnvironmentThemePriorityStackEntry {
 float BaseWeight; bool BaseWeightLocked;
 struct CThemeWeight; CThemeWeight* ThemesFirst; CThemeWeight* ThemesLast; CThemeWeight* ThemesCapacity;
 long ThemeTransitionTargetID; float ThemeTransitionInitialWeight; bool ThemeTransitionActive;
 double ThemeTransitionStartTime; float ThemeTransitionDuration;
 void SetOnlyTheme(long); void RestoreDefaultTheme(); void SetThemeWeight(long,float,bool); void RebalanceWeights();
 __forceinline void SetBaseWeight(float weight,bool locked) {
   if(weight < 0.0f) weight=0.0f; else if(weight > 1.0f) weight=1.0f;
   BaseWeight=weight; BaseWeightLocked=locked; RebalanceWeights();
 }
 void ProcessTransitions();
};
void CEnvironmentThemePriorityStackEntry::ProcessTransitions() {
 if(ThemeTransitionActive) {
  double now=GFGetTime();
  if(now > ThemeTransitionStartTime+ThemeTransitionDuration) {
   if(ThemeTransitionTargetID>=0) SetOnlyTheme(ThemeTransitionTargetID); else RestoreDefaultTheme();
   ThemeTransitionActive=false;
  } else {
   float weight=ThemeTransitionInitialWeight+(1.0f-ThemeTransitionInitialWeight)*((now-ThemeTransitionStartTime)/ThemeTransitionDuration);
   if(ThemeTransitionTargetID>=0) SetThemeWeight(ThemeTransitionTargetID,weight,true); else SetBaseWeight(weight,true);
  }
 }
}
}