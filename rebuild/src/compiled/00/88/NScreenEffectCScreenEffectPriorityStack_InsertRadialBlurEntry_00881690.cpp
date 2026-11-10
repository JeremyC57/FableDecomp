// 00881690 NScreenEffect::CScreenEffectPriorityStack::InsertRadialBlurEntry -- append and index a timed radial blur.
// Leaf: ?InsertRadialBlurEntry@CScreenEffectPriorityStack@NScreenEffect@@QAEPAXMABUCRadialBlurParams@CEnvironmentSpellEffectInterface@@ABUCRadialBlurPositionParams@4@W4ScreenEffectPriority@2@@Z
// Retail:00881690,229 bytes,thiscall.
#define FABLE_RETAIL_CSCREENEFFECTPRIORITYSTACK_FULL
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "engine/CMainGameComponent.h"
#include "engine/NScreenEffect_CScreenEffectPriorityStack.h"
#ifdef _MSC_VER
#pragma auto_inline(off)
#endif
NScreenEffect::CScreenEffectPriorityStack::CRadialBlurEntry::CRadialBlurEntry(const CRadialBlurEntry& other)
 :CStackEntry(other),param(other.param),posparam(other.posparam){}
#ifdef _MSC_VER
#pragma auto_inline(on)
#endif
void* NScreenEffect::CScreenEffectPriorityStack::InsertRadialBlurEntry(float dur_secs,const CEnvironmentSpellEffectInterface::CRadialBlurParams& p,const CEnvironmentSpellEffectInterface::CRadialBlurPositionParams& pp,ScreenEffectPriority pri)
{
 long d=(long)(dur_secs*CMainGameComponent::GetConstantFPS());
 RadialParamStack[pri].push_back(CRadialBlurEntry(p,pp,d));
 CStackEntry* e=&RadialParamStack[pri].back();
 RadialBlurKeys.insert(e);
 void* k=e;
 return k;
}