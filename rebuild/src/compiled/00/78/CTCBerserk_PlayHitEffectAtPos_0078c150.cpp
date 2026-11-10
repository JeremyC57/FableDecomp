// CTCBerserk::PlayHitEffectAtPos -- select and spawn a berserk hit effect.
// Leaf: ?PlayHitEffectAtPos@CTCBerserk@@QBEXABVC3DVector@@@Z
// Retail: Fable.exe 0x0078C150,192 bytes.
#define FABLE_RETAIL_CTCBERSERK_HIT_EFFECT
#define FABLE_RETAIL_CTCSPECIALABILITIES_FULL
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_TYPENAME
#include "engine/C3DVector.h"
#include "fable_string.h"
#include "engine/CSpecialAbilitiesBerserkDef.h"
#include "engine/CSpecialEffectsDef.h"
#include "engine/CDefClassBase_GetSubDef.h"
#include "engine/CTCBerserk.h"
#include "engine/CTCSpecialAbilities.h"
#include "engine/CWorld.h"
#include "engine/GFFloatRandom.h"
void CTCBerserk::PlayHitEffectAtPos(const C3DVector& pos)const
{
 CDefPointer<const CSpecialEffectsDef> pspecial_effects;
 long effect=0;
 if(PDef->GetSubDef(&pspecial_effects)) {
  if(GFFloatRandom(1.0f,PeekWorld().GetWorldSeed())>0.5f) {
   if(pspecial_effects->GetEffectFromString("BERSERK_HIT_EFFECT_01",effect)) {
    CThing* pfx=CTCSpecialAbilities::CreateParticleEffect(effect,pos);
   }
  } else {
   if(pspecial_effects->GetEffectFromString("BERSERK_HIT_EFFECT_02",effect)) {
    CThing* pfx=CTCSpecialAbilities::CreateParticleEffect(effect,pos);
   }
  }
 }
}