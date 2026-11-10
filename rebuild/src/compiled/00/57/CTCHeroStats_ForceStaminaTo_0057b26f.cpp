// CTCHeroStats::ForceStaminaTo(float) -- set rounded stamina percentage, restart refill delay and cancel shield at zero
// Leaf: ?ForceStaminaTo@CTCHeroStats@@QAEXM@Z
// Retail: 0x0057B26F,156 bytes,thiscall
// Retail-asm: 0x0057B29C fistp dword via original shared GFFloatToLongNear.
// Flags: /O1 /Oy-
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTCHEROSTATS
#define FABLE_RETAIL_CTCHEROSTATS_STAMINA
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER_PEEKHEROSTATSDEF
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER_HEROSTATSDEF
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#include "engine/CThing.h"
#include "engine/CTCHeroStats.h"
#include "engine/CTCPhysicalShieldSpell.h"
#include "engine/CHeroStatsDef.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CMainGameComponent.h"
#include "engine/GFFloatToLongNear.h"
template<class T,class L,class H>void FABLE_FASTCALL GFLimit(T& value,const L& lo,const H& hi){if(value<lo)value=lo;else if(value>hi)value=hi;}
void CTCHeroStats::ForceStaminaTo(float percentage){
 GFLimit(percentage,0.0f,1.0f);
 Stamina=GFFloatToLongNear(MaxStamina*percentage);
 FramesUntilStaminaRefills=(long)(GetDefinitionManager().PeekHeroStatsDef().NumberOfSecondsAfterUseStaminaStartsRefilling*CMainGameComponent::GetConstantFPS());
 if(Stamina==0){
  CTCPhysicalShieldSpell* listener;
  if(ParentThing->GetTC(TCI_PHYSICAL_SHIELD_SPELL,&listener))listener->CancelMove();
 }
}