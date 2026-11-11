// CTCHitLocations::GetBoundingSphere -- combine hit spheres and cache the centre relative to the parent position
// Retail: 00815420,498 bytes; const thiscall,ret12.
// Leaf: ?GetBoundingSphere@CTCHitLocations@@QBEXAAVC3DVector@@AAM_N@Z
// Flags: none
#define FABLE_RETAIL_HITLOCATIONS_BOUND
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_ACCURATE_MAGNITUDE
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE_SQRTF
#include "engine/CThing.h"
#include "engine/CTCHitLocations.h"
void CTCHitLocations::GetBoundingSphere(C3DVector& centre_pos,float& radius,bool accurate)const
{
 if(HitLocations.empty())return;
 if(!accurate&&CachedBoundingSphere) {
  const C3DVector& pos=GetParentThing().GetPos();
  centre_pos=pos+BoundingSphereOffsetFromPos;
  radius=BoundingSphereRadius;
  return;
 }
  std::vector<std::pair<NHitLocations::CHitLocation,long> >::const_iterator it=HitLocations.begin();
  (it++)->first.GetBoundingSphere(centre_pos,radius);
  for(;it!=HitLocations.end();++it) {
   C3DVector this_centre;
   float this_rad;
   it->first.GetBoundingSphere(this_centre,this_rad);
   C3DVector line_to_this_centre=this_centre-centre_pos;
   float centre_sep=line_to_this_centre.GetAccurateMagnitude();
   if(centre_sep>DELTA_NOTIONAL_ZERO)line_to_this_centre/=centre_sep;
   if(!(this_rad+centre_sep<radius)) {
    float new_radius=(centre_sep+this_rad+radius)/2.0f;
    float offset=new_radius-radius;
    if(offset<0.0f)offset=0.0f;
    centre_pos+=line_to_this_centre*offset;
    radius=new_radius;
   }
  }
  BoundingSphereOffsetFromPos=centre_pos-GetParentThing().GetPos();
  BoundingSphereRadius=radius;
  CachedBoundingSphere=true;
}