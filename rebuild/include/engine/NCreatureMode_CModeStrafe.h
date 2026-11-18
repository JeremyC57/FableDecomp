#ifndef FABLE_ENGINE_NCREATUREMODE_CMODESTRAFE_H
#define FABLE_ENGINE_NCREATUREMODE_CMODESTRAFE_H
#ifdef FABLE_RETAIL_NCREATUREMODE_CMODESTRAFE_PASS24
#include "engine/CTCCreatureModeManager.h"
#include "engine/CAnimFilterOfGroup.h"
#include "engine/CCountedPointer.h"
#include "engine/NAnimationEntryBodge_CAnimEntryLite.h"
#include "engine/EMovementType.h"
class CAnimateThing;
class CThingCreatureBase;
namespace NCreatureMode {
CCharString FABLE_FASTCALL GetStrafingAnimationName(const CThing& thing, EMovementType move_type);
bool FABLE_FASTCALL GetAnimDistanceAndIndexFromAnimName(const CThingCreatureBase& creature, const CCharString& name, long& index, float& distance);
class CModeStrafe : public CTCCreatureModeManager::CModeBase {
public:
    virtual void AnimUpdate(CAnimateThing& anim);
private:
    bool GetBestFrameInAimingAnim(float& frame) const;   // 0x008E9460
    float         AnimMovementDistance;                  // +0x1c
    CCharString   StrafeAnimName;                        // +0x20
    long          AnimIndex;                             // +0x24
    bool          LastUsable;                            // +0x28
    CAnimFilterOfGroup Filter;                           // +0x2c CAnimFilterOfGroup
    long          ActiveWeapon;                          // +0x38
    long          AimingNumFrames;                       // +0x3c
    long          AimingCenterFrame;                     // +0x40
    float         RumbleParam;                           // +0x44
    float         PullBackAmount;                        // +0x48
    bool          ForceAnimNoBlendIn;                    // +0x4c
    CCountedPointer<NAnimationEntryBodge::CAnimEntryLite> PAnimProjectileAim;        // +0x50
    CCountedPointer<NAnimationEntryBodge::CAnimEntryLite> PAnimProjectileBuildUp;    // +0x58
    CCountedPointer<NAnimationEntryBodge::CAnimEntryLite> PAnimProjectilePullBack;   // +0x60
    CCountedPointer<NAnimationEntryBodge::CAnimEntryLite> PAnimStrafeUpperBody;      // +0x68
};

FABLE_STATIC_ASSERT(sizeof(CModeStrafe) == 0x70);
}
#endif
#endif
