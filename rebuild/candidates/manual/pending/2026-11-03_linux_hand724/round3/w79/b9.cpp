// NCameraModes::CProjectileWeapon::Update() -- vtable 0x01277804 slot 1. Retail 0x00879900, 3415 bytes, plain `ret`.
// Leaf: ?Update@CProjectileWeapon@NCameraModes@@UAEXXZ
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_NCAMERAMODES_CBASE
#define FABLE_RETAIL_CUSERPROFILEMANAGER
#define FABLE_RETAIL_CTCHERORANGEDATTACKS
#define FABLE_RETAIL_CTCTARGETINGPLAYER
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#include <math.h>
#include "engine/NCameraModes_CProjectileWeapon.h"
#include "engine/NCameraModes_CTools.h"
#include "engine/CThingPhysical.h"
#include "engine/CThingGameObject.h"
#include "engine/CTCHitLocations.h"
#include "engine/CTCTargetingBase.h"
#include "engine/CTCTargetingPlayer.h"
#include "engine/CTCHeroRangedAttacks.h"
#include "engine/C2DBoxI.h"
#include "engine/C2DCoordI.h"
#include "engine/CThingSearchTools.h"
#include "engine/CWorld.h"
#include "fable_user_profile.h"

extern bool g_CUnprovenCameraInputFlag_013baf98;           // 0x013BAF98 (landed CameraState::ResetCameraInputFlag 0x00878A50)
extern const float DELTA_NOTIONAL_ZERO;                    // 0x0129BA3C ?DELTA_NOTIONAL_ZERO@@3MB


class CThingFilter_IsNot {                                 // FableWin ??0CThingFilter_IsNot@@QAE@ABVCThing@@@Z (no header yet)
public:
    CThingFilter_IsNot(const CThing& thing) : PThing(&thing) {}
    const CThing* PThing;
};

namespace NCameraModes {
void CProjectileWeapon::Update()
{
    bool fovZoom = GetCameraModeDef()->ProjectileCameraFOVZoom;
    const CTCTargetingBase* targeting = 0;
    GetObservedThing()->PeekTC(&targeting);
    const CThing* target = targeting->GetPBestThingForProjectileTargetingLock();
    C2DVector stick = GetLeftStickVector();
    if (!CUserProfileManager::Get().IsCameraFPBowYZInverted())
        stick.Y = -stick.Y;
    if (CUserProfileManager::Get().IsCameraFPBowXYInverted())
        stick.X = -stick.X;
    if (g_CUnprovenCameraInputFlag_013baf98)
        stick.Y *= -1.0f;
    const CTCHeroRangedAttacks* rangedAttacks;
    GetObservedThing()->PeekTC(&rangedAttacks);
    float zoomScale = ZoomInFOV * 4.5f;
    float distance = 0.0f;
    float xCoupling = zoomScale * GetCameraModeDef()->XAxisDisplacementCoupling;
    float xDamping = GetCameraModeDef()->XAxisVelocityDamping;
    float yCoupling = zoomScale * GetCameraModeDef()->YAxisDisplacementCoupling;
    float yDamping = GetCameraModeDef()->YAxisVelocityDamping;
    float sensitivity = CUserProfileManager::Get().GetCameraSensitivity();
    if (sensitivity < 0.0f) sensitivity = 0.0f; else if (sensitivity > 1.0f) sensitivity = 1.0f;
    sensitivity += 0.5f;
    xCoupling *= sensitivity;
    yCoupling *= sensitivity;
    const CTCHitLocations* hitLocations;
    if (target && !target->IsDead() && rangedAttacks->ProjectileWeaponZLockPressure > 0.0f && target->PeekTC(&hitLocations)) {
        C3DVector centre;
        float radius;
        hitLocations->GetBoundingSphere(centre, radius, true);
        float distanceToTarget = (centre - GetObservedThing()->GetPos()).GetMagnitude();
        C3DVector targetPos = centre;
        distance = distanceToTarget;
        C3DVector offset = GetObservedThing()->GetPos() - targetPos;
        C3DVector side = C3DVector(0.0f, 0.0f, 1.0f).Cross(C3DVector(offset.X, offset.Y, 0.0f));
        side.AccurateNormalise();
        C3DVector leftPos = targetPos + C3DVector(0.0f, 0.0f, 1.0f) * radius - side * radius;
        C3DVector rightPos = targetPos - C3DVector(0.0f, 0.0f, 1.0f) * radius + side * radius;

        float leftPhi = GetPhiForPosition(leftPos);
        float rightPhi = GetPhiForPosition(rightPos);
        float leftHeight = GetHeightParamForPostionWithPhi(leftPos, leftPhi);
        float rightHeight = GetHeightParamForPostionWithPhi(rightPos, rightPhi);
        float maxHeight = leftHeight > rightHeight ? leftHeight : rightHeight;
        float minHeight = leftHeight < rightHeight ? leftHeight : rightHeight;
        if (maxHeight > 1.0f) maxHeight = 1.0f;
        if (minHeight < 0.0f) minHeight = 0.0f; else if (minHeight > maxHeight) minHeight = maxHeight;
        float phi = GetPhiForTargetedThing(*target);
        float heightParam = GetHeightParamForTargetedThingWithPhi(*target, phi);
        Phi = (Phi * 3.0f + phi) * 0.25f;
        HeightParam = (HeightParam * 3.0f + heightParam) * 0.25f;
        DeltaLockDeltaPhi += (xCoupling * stick.X - xDamping * DeltaLockDeltaPhi) * 0.2f;
        DeltaLockDeltaHeightParam += (yCoupling * stick.Y - yDamping * DeltaLockDeltaHeightParam) * 0.2f;
        float lockPhi = DeltaLockDeltaPhi * 0.2f + LockDeltaPhi;
        float lockHeight = DeltaLockDeltaHeightParam * 0.2f + LockDeltaHeightParam;
        float minPhi = leftPhi - phi, maxPhi = rightPhi - phi;
        if (lockPhi < minPhi) lockPhi = minPhi; else if (lockPhi > maxPhi) lockPhi = maxPhi;
        float minLock = minHeight - heightParam, maxLock = maxHeight - heightParam;
        if (lockHeight < minLock) lockHeight = minLock; else if (lockHeight > maxLock) lockHeight = maxLock;
        DeltaPhi = 0.0f;
        DeltaHeightParam = 0.0f;
        float testPhi = lockPhi + phi;
        float testHeight = lockHeight + heightParam;
        C3DVector testPos = GetPosFromParams(testPhi, testHeight);
        C3DVector testLook = GetLookDirectionFromParams(testPhi, testHeight);
        std::vector<const CThing*> ignore;
        ignore.push_back(target);
        C3DVector testEnd = testPos + testLook * (radius * 2.0f + distanceToTarget);
        C3DVector hitPos, hitNormal;
        float hitDistance;
        const CThing* hitThing = 0;
        if (!GetCamTools().IsLineOfSight(ignore, testPos, testEnd, hitPos, hitNormal, hitDistance, &hitThing, (CTools::ELOSTest)15, false)
            && hitThing && !hitThing->IsDead()) {
            LockDeltaPhi = lockPhi;
            LockDeltaHeightParam = lockHeight;
        } else {
            DeltaLockDeltaPhi = 0.0f;
            DeltaLockDeltaHeightParam = 0.0f;
        }
    } else {
        Phi += LockDeltaPhi;
        LockDeltaPhi = 0.0f;
        DeltaLockDeltaPhi = 0.0f;
        DeltaLockDeltaHeightParam = 0.0f;
        HeightParam += LockDeltaHeightParam;
        LockDeltaHeightParam = 0.0f;

        DeltaPhi += (xCoupling * stick.X - xDamping * DeltaPhi) * 0.2f;
        DeltaHeightParam += (yCoupling * stick.Y - yDamping * DeltaHeightParam) * 0.2f;
    }
    Phi += DeltaPhi * 0.2f;
    HeightParam += DeltaHeightParam * 0.2f;
    if (HeightParam < 0.0f) HeightParam = 0.0f; else if (HeightParam > 1.0f) HeightParam = 1.0f;
    float phi = Phi + LockDeltaPhi;
    float heightParam = LockDeltaHeightParam + HeightParam;
    if (heightParam < 0.0f) heightParam = 0.0f; else if (heightParam > 1.0f) heightParam = 1.0f;
    C3DVector position = GetPosFromParams(phi, heightParam);
    C3DVector lookDirection = GetLookDirectionFromParams(phi, heightParam);
    float lookDistance = 1.0f;
    if (lookDirection.GetSquaredMagnitude() > DELTA_NOTIONAL_ZERO * DELTA_NOTIONAL_ZERO) {
        lookDirection.AccurateNormalise();
        const CTCTargetingPlayer* targetingPlayer;
        if (GetObservedThing()->PeekTC(&targetingPlayer))
            lookDistance = targetingPlayer->GetMaxTargetingDistance();
    }
    C2DVector positionXY(position.X, position.Y);
    C3DVector lookAt = position + lookDirection * lookDistance;
    float zoomOffset = GetCameraModeDef()->ProjectileCameraDefaultTranslationalZoomOffset;
    if (distance < zoomOffset) distance = zoomOffset;
    C3DVector zoom = lookDirection * zoomOffset;
    if (!fovZoom)
        zoom += lookDirection * (distance - zoomOffset);
    float zoomParameter = 1.0f;
    if (rangedAttacks->IsUsingProjectileTargetingAnalogueZoom())
        zoomParameter = rangedAttacks->GetProjectileTargetingZoomParameter();
    float fov = 0.22222222f;
    if (zoomParameter >= 0.0f)
        fov = (MaxZoomInFOV - 0.22222222f) * zoomParameter + 0.22222222f;
    ZoomInFOV = (ZoomInFOV * 3.0f + fov) * 0.25f;
    SetFOV(ZoomInFOV);
    position += zoom;
    C3DVector side(lookDirection.X, lookDirection.Y, 0.0f);
    side.AccurateNormalise();
    side = C3DVector(side.Y, -side.X, 0.0f);
    position += side * (float)(exp(heightParam * heightParam * -2.0f) * 0.25);
    C3DVector observedPos = GetObservedThing()->GetPos();
    observedPos.Z = position.Z;

    std::vector<const CThing*> nearby;
    CThingFilter_IsNot notObserved(*GetObservedThing());
    long x = (long)positionXY.X;
    long y = (long)positionXY.Y;
    C2DBoxI box(C2DCoordI(x, y), 2);
    GetWorld().PeekThingSearchTools().GetAllThingsInRoughArea<const CThing>(box, notObserved, nearby);

    C3DVector hitPos, hitNormal;
    float hitDistance;
    const CThing* hitThing;
    if (!GetCamTools().IsLineOfSight(nearby, observedPos, position, hitPos, hitNormal, hitDistance, &hitThing, (CTools::ELOSTest)8, false))
        position = hitPos;
    SetPosition(position);
    SetObservedPos(lookAt);
}
} // namespace NCameraModes
