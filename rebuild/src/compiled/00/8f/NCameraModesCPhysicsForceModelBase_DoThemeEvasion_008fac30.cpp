// NCameraModes::CPhysicsForceModelBase::DoThemeEvasion() -- sample the theme potential gradient around the camera.
// Retail: 0x008FAC30, 372 bytes; thiscall.
// Grade: VERIFIED_MATCH; see ROW.json and PROBES.md for verification or remaining residue.
// Leaf: ?DoThemeEvasion@CPhysicsForceModelBase@NCameraModes@@UAE?AVC3DVector@@XZ
// Flags: /d2inlT400
// Inline-budget: /d2inlT400 -- expand the original potential-field helper.
#define FABLE_RETAIL_STLP_LIST_BASE_DTOR_VISIBLE_NOINLINE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_NCAMERAMODES_CBASE_POSITION
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CGAMECAMERAMANAGER_WORLD
#define FABLE_RETAIL_CPHYSICSCOLLISIONPOINTSOBJECT_POINTS
#include "engine/NCameraModes_CPhysicsForceModelBase.h"
#include "engine/CWorld.h"
C3DVector NCameraModes::CPhysicsForceModelBase::DoThemeEvasion()
{
 std::list<C3DVector> theme_points;
 C3DVector cam_pos=GetPosition();
 const CWorld& world=GetCameraManager().GetWorld();
 GetCollisionPoints<const CWorldMap>(Themes,world.PeekWorldMap(),cam_pos,2,50.0f);
 C3DVector return_value=PotentialFieldBufferPoints(cam_pos,Themes.Points,50.0f,0.0f);
 return return_value;
}