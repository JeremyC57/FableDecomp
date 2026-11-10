// NCameraModes::CMoveOnCylinder::OnInit -- initialise the camera cylinder coordinates.
// Retail: 0x0085C7D0, 332 bytes; thiscall.
// Grade: VERIFIED_MATCH; see ROW.json and PROBES.md for verification or remaining residue.
// Leaf: ?OnInit@CMoveOnCylinder@NCameraModes@@UAEXXZ
// Flags: none
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_NCAMERAMODES_CBASE_CYLINDER
#define FABLE_RETAIL_NCAMERAMODES_CPHYSICSLEAF_CYLINDER
#define FABLE_RETAIL_CGAMECAMERAMANAGER
#define FABLE_RETAIL_CGAMECAMERAMANAGER_SINGLECUT
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_C3DVECTOR_CYLINDER_CROSS_GROUPED
#define FABLE_RETAIL_C3DVECTOR
#include "engine/NCameraModes_CMoveOnCylinder.h"
#include "engine/NCameraModes_CPhysicsLeaf.h"
#include "engine/CGameCameraManager.h"
#include "engine/CThingGameObject.h"
#include "engine/CTCPhysicsBase.h"
void NCameraModes::CMoveOnCylinder::OnInit()
{
 AddSubComponent(CCountedPointer<CSubComponentBase>(new CPhysicsLeaf<CPhysicsTargetModelGoTo,CPhysicsForceModelRigid,CPhysicsCoordRigid>(*this,GetCameraManager())),false);
 AxisX=GetObservedThing()->GetPhysicsTC()->GetFacingVector();
 AxisZ=C3DVector(0.0f,0.0f,1.0f);
 AxisY=AxisX.Cross(AxisZ);
 Theta=0.0f;
 HeightParam=0.75f;
 GetCameraManager().SetSingleCut();
}