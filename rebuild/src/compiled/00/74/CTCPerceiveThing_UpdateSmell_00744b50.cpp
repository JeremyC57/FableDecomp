// CTCPerceiveThing::UpdateSmell -- update scent detection using range and wind direction.
// Retail: 0x00744B50, 349 bytes; thiscall.
// Grade: VERIFIED_MATCH; see ROW.json and PROBES.md for verification or remaining residue.
// Leaf: ?UpdateSmell@CTCPerceiveThing@@AAEXXZ
// Flags: none
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCPERCEIVETHING_SMELL
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_ENVIRONMENT
#define FABLE_RETAIL_CWORLD_PEEKENVIRONMENT
#define FABLE_RETAIL_CENVIRONMENT_VIRTUALS
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_GROUPED_SUM
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_GFFASTONEOVERSQRT_ORIGINAL_UNION
#include "engine/CTCPerceiveThing.h"
#include "engine/CWorld.h"
#include "engine/CEnvironment.h"
#include "engine/GFCos.h"
void CTCPerceiveThing::UpdateSmell()
{
 float distance_sq=GFSqr(GetSmellRadius());
 Smelt=false;
 if(DistanceFromPerceivedThingSquared<distance_sq) {
  const CThing* pperceived_thing=GetPerceivedThing();
  C3DVector wind_direction=PeekWorld().PeekEnvironment().PeekWindDirection();
  C3DVector between=GetParentThing().GetPos()-pperceived_thing->GetPos();
  between.Normalise();
  float cos_smell_angle_cone=GFCos(70.0f/360.0f);
  if(wind_direction.Dot(between)>cos_smell_angle_cone) {
   Smelt=true;
   if(!IsThingLineOfSight()) SetAsDirectlyAwareOfPerceivedThing();
  }
 }
}