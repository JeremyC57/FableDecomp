// NCameraModes::CMoveOnCylinder::IsRaiseUpOnHeroNearby() const -- returns false
// Leaf: ?IsRaiseUpOnHeroNearby@CMoveOnCylinder@NCameraModes@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00637810, thiscall, `ret`.
// Identity: slot 7 of NCameraModes::CMoveOnCylinder's vtable 0x0125A064 (defined here: the slot holds this body in 1 vtable(s), all of NCameraModes::CMoveOnCylinder or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 8 TLC-named slots of that vtable puts IsRaiseUpOnHeroNearby(void) at this slot; Ego_r/FableWin name set: ?IsRaiseUpOnHeroNearby@CMoveOnCylinder@NCameraModes@@UBE_NXZ; `ret` matches the signature.
// Header: engine/NCameraModes_CMoveOnCylinder.h (opt-ins as in the landed row NCameraModesCMoveOnCylinder_OnInit_0085c7d0.cpp).
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
bool NCameraModes::CMoveOnCylinder::IsRaiseUpOnHeroNearby() const
{
    return false;
}
