// CTCStealth::GetPositionToMoveToForStabOn -- compute the approach position for a stealth stab.
// Retail: 0x00746EC0, 326 bytes; thiscall.
// Grade: VERIFIED_MATCH; see ROW.json and PROBES.md for verification or remaining residue.
// Leaf: ?GetPositionToMoveToForStabOn@CTCStealth@@QBE?AVC3DVector@@AAVCThing@@AAM@Z
// Flags: none
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_ANGLE_BODIES
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_GFFASTONEOVERSQRT_ORIGINAL_UNION
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CTCStealth.h"
C3DVector CTCStealth::GetPositionToMoveToForStabOn(CThing& thing_to_stab,float& new_facing_angle)const
{
 C3DVector sword_stab_pos=NStealth::GetStabPosOnThing(thing_to_stab);
 C3DVector to_stab_pos=sword_stab_pos-GetParentThing().GetPos();
 C3DVector to_stand_pos(to_stab_pos);
 to_stand_pos.Normalise();
 C3DVector standing_stab_pos=sword_stab_pos-to_stand_pos;
 new_facing_angle=to_stand_pos.GetAngleXY();
 return standing_stab_pos;
}