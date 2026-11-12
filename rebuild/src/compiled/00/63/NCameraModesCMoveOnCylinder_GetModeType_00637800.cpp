// NCameraModes::CMoveOnCylinder::GetModeType() const -- returns (EMode)0x12
// Leaf: ?GetModeType@CMoveOnCylinder@NCameraModes@@UBE?AW4EMode@2@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00637800, thiscall, `ret`.
// Identity: slot 2 of NCameraModes::CMoveOnCylinder's vtable 0x0125A064 (defined here: the slot holds this body in 1 vtable(s), all of NCameraModes::CMoveOnCylinder or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 8 TLC-named slots of that vtable puts GetModeType(void) at this slot; Ego_r/FableWin name set: ?GetModeType@CMoveOnCylinder@NCameraModes@@UBE?AW4EMode@2@XZ; `ret` matches the signature.
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
NCameraModes::EMode NCameraModes::CMoveOnCylinder::GetModeType() const
{
    return (EMode)0x12;
}
