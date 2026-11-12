// CGameCameraBase::Update() -- per-frame orbit camera: keep last frame's camera, aim one unit above the live hero, orbit it at theta / phi / radius (eased towards the fixed values, theta raised with the controlled creature's speed, phi turned towards the view from behind the creature or behind the nearby things), honour the fixed-camera and thing-collision overrides, then keep the eye in line of sight (else 1.5 above the last unblocked ground point)
// Leaf: ?Update@CGameCameraBase@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006FDD60, 1659 bytes, `ret` (int3 padding 0x006FE3DB..0x006FE3DF, then uncatalogued STLport
//   list<C3DVector> COMDATs 0x006FE3E0..0x006FE53F: empty, iterator, front, begin / end, allocate / deallocate). Vtable 0x01264A8C slot 0.
// Names (two sources each, NOTES.md): Ego_r.pdb / FableWin.pdb ?Update@CGameCameraBase@@UAEXXZ; the members in Ego_r donor order equal
//   retail order (GetRotationAngle 0x006FD4F0, IsThingCollision 0x006FDA20 = ?IsThingCollision@CGameCameraBase@@AAE_NXZ, private).
// Callees: CWorld::GetFrame 0x0049D870 (static), CWorld::PeekMyPlayer 0x0049D9D0, CBaseIntelligentPointer::GetPItem 0x00A01B50 (the
//   inline CPlayer::GetPPlayerCharacter, tested by IS_THING_ALIVE: null, then CThing::Dead bit 0 of +0x91), CThingPhysical::PeekPos
//   0x006A4D00, CPlayer::GetPControlledCreature 0x00487DC0 (its PhysicsTC +0x60: Position +0x0C, OldPosition +0x18, GetFacingVector
//   slot 70), DivideByGameSpeed 0x00415FD3, GetRotationAngle 0x006FD4F0, CCamera::PointAt 0x00A0C2B0, IsThingCollision 0x006FDA20,
//   CWorld::GetWorldMap (slot 12), CWorldMap::IsLineOfSight 0x0063AEE0 and FindLastUnblockedGroundPositionAndReportCollision 0x0063D960.
//   Globals: CMainGameComponent::ConstantFPS 0x01375550, GFastOneOverSqrtTable 0x013CE558. Constants: 2pi (double) 0x0124F2B8,
//   1.0f 0x0122DED8, 0.5f 0x0122F59C, 0.0f 0x0122DEDC, 3.0f 0x0122DED4, 0.08 (double) 0x01264AF8, 0.01f 0x01230A5C, 0.0005f 0x01264AF0,
//   0.05f 0x0125A15C, 1.5f 0x0125AADC (each checked against the object's __real pool).
// Shape (each measured, NOTES.md): GFSin / GFCos of theta / phi in turns (double 2pi); both Normalise expansions are the FableWin body
//   (GFSqr helper, FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER: X, Z, Y squares at 0x006FDF54); the easing is `Rad += (RadFixed -
//   Rad) * 0.01f` (members re-read, not GFGetInterpolatedValue); `if (dot >= 0.0f) { flatten } else { theta 0.08 }` puts the flatten
//   arm after the easing block (0x006FE1CB) as retail; GetRotationAngle's body is visible in the TU (noinline) so `behind` / `offset`
//   share frame words with later temporaries; the list element is read through a named `const C3DVector& pos` (fixes the x87
//   operand order of the flattened Normalise, 0x006FE22E `fst ; fmulp st(1)`); the blocked branch assigns `new_pos` and one
//   MoveTo follows (VC7.1 duplicates it into both exits with retail's eax / ecx / edx rotation).
#define FABLE_RETAIL_CGAMECAMERABASE_RENDER
#define FABLE_RETAIL_CGAMECAMERABASE_UPDATE
#define FABLE_RETAIL_CAIGAMECAMERABASE
#define FABLE_RETAIL_CCAMERA_RENDER_INTERPOLATION
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSBASE_LASTPOSITION
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CWORLDMAP_LINEOFSIGHT
#define FABLE_RETAIL_CWORLDMAP_FINDLASTUNBLOCKEDGROUND
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_IS_THING_ALIVE
extern "C" double __cdecl sqrt(double);
extern "C" double __cdecl sin(double);
extern "C" double __cdecl cos(double);
extern "C" double __cdecl acos(double);
#pragma intrinsic(sqrt, sin, cos, acos)
#include "engine/CWorldMap.h"                   // overlay copy first (FindLastUnblockedGroundPositionAndReportCollision opt-in)
#include "fable_string.h"
#include "engine/CGameCameraBase.h"
#include "engine/CThing.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CWorld.h"
#include "engine/CPlayer.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CMainGameComponent.h"
#include "engine/ThingAlive.h"
#include "engine/GFSin.h"
#include "engine/GFCos.h"

float __stdcall DivideByGameSpeed(float value);              // 0x00415FD3 ?DivideByGameSpeed@@YGMM@Z

// Same-TU body of the landed private member 0x006FD4F0 (retail compiles it in this TU, ahead of Update, and calls it out of
// line from Update only, 0x006FE1AF): visible so VC7.1 knows it only reads the two vectors whose addresses Update passes.
#ifdef _MSC_VER
__declspec(noinline)
#endif
float CGameCameraBase::GetRotationAngle(const C3DVector& from, const C3DVector& to)
{
    float result = 0.0f;
    C3DVector flatTo(to.X, to.Y, 0.0f);
    float cosAngle = flatTo.Dot(from) / (from.GetMagnitude() * flatTo.GetMagnitude());
    if (cosAngle <= 1.0f && cosAngle >= -1.0f) {
        float angle = (float)acos(cosAngle) * 0.15915494f;
        float sign = 1.0f;
        if (flatTo.X * from.Y - flatTo.Y * from.X < 0.0f)
            sign = -1.0f;
        result = angle * sign;
    }
    return result;
}

void CGameCameraBase::Update()
{
    LastInternalCam = InternalCam;
    LastPointingAt = PointingAt;
    long frame = World.GetFrame();
    C3DVector offset(Rad * GFCos(Phi) * GFSin(Theta), Rad * GFSin(Phi) * GFSin(Theta), Rad * GFCos(Theta));
    if (!IS_THING_ALIVE(World.PeekMyPlayer()->GetPPlayerCharacter()))
        return;
    C3DVector target = World.PeekMyPlayer()->GetPPlayerCharacter()->PeekPos() + C3DVector(0.0f, 0.0f, 1.0f);
    C3DVector velocity = (World.PeekMyPlayer()->GetPControlledCreature()->GetPhysicsTC()->GetPosition() -
                          World.PeekMyPlayer()->GetPControlledCreature()->GetPhysicsTC()->GetLastPosition()) *
                         (float)CMainGameComponent::GetConstantFPS();
    C3DVector facing = World.PeekMyPlayer()->GetPControlledCreature()->GetPhysicsTC()->GetFacingVector();
    if (DivideByGameSpeed((float)(frame - TriggerFrame)) > 0.5f && BuildingUID == 0 && ActAsFixedCam) {
        ActAsFixedCam = false;
        Rad = (PointingAt - InternalCam.PeekWorldPos()).GetMagnitude();
    }
    float theta_offset = 0.0f;
    facing.Normalise();
    C3DVector behind = -facing * Rad;
    if (!LocalThingPosList.empty()) {
        C3DVector total(0.0f, 0.0f, 0.0f);
        std::list<C3DVector>::const_iterator it = LocalThingPosList.begin();
        C3DVector last = *it - target;
        float dot = 0.0f;
        for (; it != LocalThingPosList.end(); ++it) {
            const C3DVector& pos = *it;
            C3DVector rel = pos - target;
            total += rel;
            if (dot >= 0.0f)
                dot = rel.Dot(last);
            last = rel;
        }
        if (dot >= 0.0f) {
            total.Z = 0.0f;
            total.Normalise();
            behind = -total * Rad;
        } else {
            theta_offset = 0.08 - ThetaFixed;
        }
    }
    if (!ActAsFixedCam) {
        Rad += (RadFixed - Rad) * 0.01f;
        Theta += (velocity.GetMagnitude() * 0.0005f + ThetaFixed + theta_offset - Theta) * 0.05f;
        Phi += GetRotationAngle(behind, offset) * PhiChanger;
    }
    C3DVector new_pos = InternalCam.PeekWorldPos();
    if (!ActAsFixedCam)
        new_pos = target + offset;
    PointingAt = target;
    InternalCam.PointAt(PointingAt);
    if (IsThingCollision()) {
        new_pos = DrawCoordList.front();
        ActAsFixedCam = true;
    }
    if (!World.GetWorldMap().IsLineOfSight(PointingAt, new_pos)) {
        C3DVector collision;
        World.GetWorldMap().FindLastUnblockedGroundPositionAndReportCollision(PointingAt, new_pos, collision);
        new_pos = collision + C3DVector(0.0f, 0.0f, 1.5f);
    }
    InternalCam.MoveTo(new_pos);
}
