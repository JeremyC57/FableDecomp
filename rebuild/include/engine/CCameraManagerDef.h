#ifndef FABLE_ENGINE_CCAMERAMANAGERDEF_H
#define FABLE_ENGINE_CCAMERAMANAGERDEF_H

#ifdef FABLE_RETAIL_CGAMECAMERAMANAGER_HEIGHTLIMITS_PASS24
#include "engine/CParentDefClassBase.h"
#include "stlport/fable_containers.h"

class CCameraManagerDef : public CParentDefClassBase {
public:
    std::vector<long> CameraList;
    std::vector<long> SpecialityCameraList;
    long InitialMode;
    long ZTargetMode;
    long ProjectileWeaponMode;
    float BaseHeightToKeepAboveFlatLandscape;
    float BaseHeightToKeepAboveWater;
    float MaxHeightRelativeToGround;
    float MaxHeightStepForPointAcceptance;
    float AngleBetweenGroundNormalAndVerticalForLandscapeToBeConsideredNotFlat;
};

FABLE_STATIC_ASSERT(offsetof(CCameraManagerDef, BaseHeightToKeepAboveFlatLandscape) == 0x60);
FABLE_STATIC_ASSERT(offsetof(CCameraManagerDef, AngleBetweenGroundNormalAndVerticalForLandscapeToBeConsideredNotFlat) == 0x70);
FABLE_STATIC_ASSERT(sizeof(CCameraManagerDef) == 0x74);
#endif
#endif
