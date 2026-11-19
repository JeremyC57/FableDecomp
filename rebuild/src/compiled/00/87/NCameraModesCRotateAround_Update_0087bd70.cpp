// NCameraModes::CRotateAround::Update() -- turn Theta by the right stick's X (inverted per the profile) x the manager's look-around delta, place the camera on the XAxis/YAxis circle of radius StringLengthXY around the observed thing's focal point (at the thing's height + HeightOffset, clamped to the world height limits unless the view is occluded), looking at the focal point
// Leaf: ?Update@CRotateAround@NCameraModes@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0087BD70, 451 bytes, thiscall, `ret` (vtable slot 1).
// Source: FableWin.pdb debug body 0x029C7B70 (cm_manual_control.obj) statement order and calls with Ego_d.pdb's locals (cam_stick,
//   cam_stick_x, foc_pos, thing_base, d_thet, line_out, new_pos): GetRightStickVector, CUserProfileManager::Get().IsCameraXYInverted,
//   GetObservedThing()->GetTCGraphicAppearance()->GetStaticCameraFocalPos(), GetPos, GetCameraManager(), GFSin / GFCos, C3DVector
//   operator* / operator+, IsCameraViewPosOccluded, GetMin/MaxWorldHeight, SetPosition, SetObservedPos.
#include "fable_compiler.h"
#include <math.h>
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_MATH_TRIG_TWO_PI
#define FABLE_RETAIL_NCAMERAMODES_CBASE_CYLINDER
#define FABLE_RETAIL_NCAMERAMODES_CBASE_INLINE_ACCESSORS
#define FABLE_RETAIL_NCAMERAMODES_CBASE_SETPOSITION_INLINE
#define FABLE_RETAIL_NCAMERAMODES_CBASE_VIEWOCCLUDED_INLINE
#define FABLE_RETAIL_PCMOUSELOOK_PASS24
#define FABLE_RETAIL_CAIGAMECAMERABASE
#define FABLE_RETAIL_CGAMECAMERAMANAGER_LOOKAROUND
#define FABLE_RETAIL_CGAMECAMERAMANAGER_WORLDHEIGHT_GETTERS
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_STATIC_FOCAL
#define FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST
#include "engine/C3DVector.h"
#include "engine/GFSin.h"
#include "engine/GFCos.h"
#include "engine/CThing.h"
#include "engine/CThingGameObject.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/CUserProfileManager.h"
#include "engine/CGameCameraManager.h"
#include "engine/NCameraModes_CRotateAround.h"

namespace NCameraModes {
void CRotateAround::Update()
{
    const C2DVector& cam_stick = GetRightStickVector();
    float cam_stick_x = cam_stick.X;
    if (CUserProfileManager::Get().IsCameraXYInverted())
        cam_stick_x *= -1.0f;
    C3DVector foc_pos = GetObservedThing()->GetTCGraphicAppearance()->GetStaticCameraFocalPos();
    C3DVector thing_base(GetObservedThing()->GetPos());
    float d_thet = GetCameraManager().CamRSLookAroundDeltaTheta * cam_stick_x;
    Theta += d_thet;
    C3DVector line_out = XAxis * (float)GFCos(Theta) + YAxis * (float)GFSin(Theta);
    C3DVector new_pos = C3DVector(foc_pos.X, foc_pos.Y, thing_base.Z) + line_out * StringLengthXY + C3DVector(0.0f, 0.0f, HeightOffset);
    if (GetCameraManager().LimitMinMaxWorldHeight && !IsCameraViewPosOccluded()) {
        if (new_pos.Z < GetCameraManager().GetMinWorldHeight())
            new_pos.Z = GetCameraManager().GetMinWorldHeight();
        else if (new_pos.Z > GetCameraManager().GetMaxWorldHeight())
            new_pos.Z = GetCameraManager().GetMaxWorldHeight();
    }
    SetPosition(new_pos);
    SetObservedPos(foc_pos);
}
}