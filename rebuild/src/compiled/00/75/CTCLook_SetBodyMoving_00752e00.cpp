// CTCLook::SetBodyMoving -- update body movement and facing state.
// Retail: 0x00752E00, 296 bytes; thiscall.
// Grade: VERIFIED_MATCH; see ROW.json and PROBES.md for verification or remaining residue.
// Leaf: ?SetBodyMoving@CTCLook@@AAEXXZ
// Flags: /d2inlT200
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_CLOOKDEF
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_NOT_EQUAL
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_IS_THING_ALIVE
#include "engine/CTCLook.h"
#include "engine/CThingCreatureBase.h"
#include "engine/ThingAlive.h"
#include "engine/GFAngles.h"
void CTCLook::SetBodyMoving()
{
 bool melee=IS_THING_ALIVE(PParentCreature) && PParentCreature->IsInMelee();
 bool dest_reached=true;
 float destination_angle=0.0f;
 float angle_from_north_to_target_xy;
 float rel_angle_difference_xy;
 if(GetTurningAnglesAndCheckForTurningBody(angle_from_north_to_target_xy,rel_angle_difference_xy)) {
  dest_reached=false;
  if(!IsLookTypeBodyOnly() && !(LookFlags&0x80)) {
   destination_angle=angle_from_north_to_target_xy-GetHeadTurnRangeXY()*GFGetSign(rel_angle_difference_xy)*0.25f;
   destination_angle=GET_ROUNDED_ANGLE(destination_angle);
  } else destination_angle=angle_from_north_to_target_xy;
 }
 if(dest_reached) {
  SetBodyTurnState(BodyTurnState|8);
  BodyVelocityXY=0.0f;
  BodyVelocityYZ=0.0f;
 } else {
  GetBodySettingsSlowInFastOut(BodyAngleXY,BodyVelocityXY,BODY_ANGULAR_ACCELERATION_XY,BODY_ANGULAR_VELOCITY_MAX_XY,destination_angle);
  ImplementBodySettings();
 }
}