#ifndef _MSC_VER
#define __cdecl
#define __forceinline inline
#endif
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

#include <stdio.h>
#include <stddef.h>
#include <limits.h>
using NEnvironmentInternal::CEnvironmentThemePriorityStackEntry;
static double timeNow; static int clockCalls,called; static long themeArg; static float weightArg;
double __cdecl GFGetTime(){++clockCalls;return timeNow;}
void CEnvironmentThemePriorityStackEntry::SetOnlyTheme(long t){called=1;themeArg=t;}
void CEnvironmentThemePriorityStackEntry::RestoreDefaultTheme(){called=2;}
void CEnvironmentThemePriorityStackEntry::SetThemeWeight(long t,float w,bool lock){called=lock?3:99;themeArg=t;weightArg=w;}
void CEnvironmentThemePriorityStackEntry::RebalanceWeights(){called=4;}
static bool run(long theme,double now,float initial,float duration,int expected,float weight){
 CEnvironmentThemePriorityStackEntry e;e.BaseWeight=-3;e.BaseWeightLocked=false;e.ThemeTransitionTargetID=theme;
 e.ThemeTransitionActive=true;e.ThemeTransitionStartTime=10;e.ThemeTransitionDuration=duration;e.ThemeTransitionInitialWeight=initial;
 called=0;clockCalls=0;timeNow=now;e.ProcessTransitions();
 if(called!=expected||clockCalls!=1)return false;
 if(expected==1||expected==2)return !e.ThemeTransitionActive;
 if(!e.ThemeTransitionActive)return false;
 if(expected==3)return themeArg==theme&&weightArg==weight;
 return e.BaseWeightLocked&&e.BaseWeight==weight;
}
int main(){
 bool ok=true;
 if(sizeof(void*)==4)ok=offsetof(CEnvironmentThemePriorityStackEntry,ThemeTransitionTargetID)==0x14&&offsetof(CEnvironmentThemePriorityStackEntry,ThemeTransitionActive)==0x1c&&offsetof(CEnvironmentThemePriorityStackEntry,ThemeTransitionStartTime)==0x20&&offsetof(CEnvironmentThemePriorityStackEntry,ThemeTransitionDuration)==0x28;
 CEnvironmentThemePriorityStackEntry idle;idle.ThemeTransitionActive=false;called=0;clockCalls=0;idle.ProcessTransitions();ok=ok&&called==0&&clockCalls==0;
 ok=ok&&run(12,12.1,.25f,2,1,0)&&themeArg==12;
 ok=ok&&run(-1,12.1,.25f,2,2,0);
 ok=ok&&run(12,12,.25f,2,3,1)&&run(12,11,.25f,2,3,.625f);
 ok=ok&&run(-1,11,.25f,2,4,.625f)&&run(-1,8,.25f,2,4,0)&&run(-1,11,2,2,4,1);
 // NaN remains unordered through both the deadline and inline clamps.
 union {unsigned int bits;float value;}nan;nan.bits=0x7fc00000;
 CEnvironmentThemePriorityStackEntry e;e.ThemeTransitionActive=true;e.ThemeTransitionTargetID=-1;e.ThemeTransitionStartTime=10;e.ThemeTransitionDuration=2;e.ThemeTransitionInitialWeight=nan.value;timeNow=11;called=0;e.ProcessTransitions();
 ok=ok&&called==4&&e.BaseWeight!=e.BaseWeight&&e.BaseWeightLocked&&e.ThemeTransitionActive;
 // Zero duration at the exact deadline also preserves the unordered division result.
 e.ThemeTransitionInitialWeight=.5f;e.ThemeTransitionDuration=0;timeNow=10;e.ProcessTransitions();ok=ok&&e.BaseWeight!=e.BaseWeight&&e.ThemeTransitionActive;
 printf(ok?"00887a30_TEST PASS\n":"00887a30_TEST FAIL\n");return ok?0:1;
}