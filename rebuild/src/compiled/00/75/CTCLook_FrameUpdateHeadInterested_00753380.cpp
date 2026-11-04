// CTCLook::FrameUpdateHeadInterested() -- per-frame head tracking of whatever the parent's targeting finds interesting: the camera when the targeting looks at it, else the nearest interesting thing's head, focal point or position; with neither, centre the head
// Leaf: ?FrameUpdateHeadInterested@CTCLook@@AAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00753380, 297 bytes, thiscall plain `ret`. FableWin.pdb ?FrameUpdateHeadInterested@CTCLook@@AAEXXZ
//   (private).
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound const 0x004365B0 (the Find expansion) inside CThing::GetTC<CTCTargetingBase> (interface 8) and
//   GetTC<CTCLook> (0x43), CTCTargetingBase::GetPNearestInterestingThing const 0x007F5860, CTCBase::GetWorld 0x00686A70 then
//   CWorld::GetGameCamera (inline, +0x34) and CAIGameCameraBase::PeekCurrentPos const (slot 11, by hidden pointer), CTCLook::GetHeadPos
//   const 0x00751B30, NGraphicAppearance::CTCGraphicAppearance::GetFocalPosIfLoaded(C3DVector&, float) const 0x004C11D0 (1.0f),
//   CTCLook::CentreHead 0x007528A0, CTCLook::FrameUpdateHeadLookAtPos(const C3DVector&, bool) 0x007530D0 (FableWin.pdb: all three
//   private, AAE / ABE).
// Shape: the inline FableWin.pdb CTCTargetingBase::IsLookingAtCamera (LookAtCamera +0x34) is tested once for the no-target exit and
//   once for the camera choice: VC7.1 threads the no-target test straight to the camera block and lays the camera block as the
//   fall-through of the target path (retail's order); the target preloaded with 0 is the dead `xor esi,esi`; a miss on the targeting
//   lookup leaves the pointer unset (retail reads the key's frame word). The thing's position is CThing::GetPhysicsTC()->GetPosition()
//   (inline const&, Position +0x0c); the medium look-speed bit (0x10) is set when none of the speed bits (0x38) is.
// Layout: CTCLook (Ego_r.pdb, 0xd4) through LookFlags +0x64; no engine header models the class (the landed CTCLook rows model it locally).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CAIGAMECAMERABASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CTCTARGETINGBASE_LOOKATCAMERA
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_FOCALPOS
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CWorld.h"
#include "engine/CAIGameCameraBase.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CTCTargetingBase.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"

class CThingCreatureBase;
class CTCLook : public CTCBase {
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return (ETCInterfaceType)0x43; }
private:
    void FrameUpdateHeadInterested();
    void CentreHead();                                              // 0x007528A0
    void FrameUpdateHeadLookAtPos(const C3DVector& pos, bool turnBody);   // 0x007530D0
    C3DVector GetHeadPos() const;                                   // 0x00751B30
    unsigned long         _pad_0x0c;                                // +0x0c
    CThingCreatureBase*   PParentCreature;                          // +0x10
    const void*           PDef;                                     // +0x14 CDefPointer<const CLookDef>
    const void*           PAppearanceDef;                           // +0x18 CDefPointer<const CAppearanceDef>
    float                 RecoveryAngleToLookAtXY;                  // +0x1c
    float                 RecoveryAngleToLookAtYZ;                  // +0x20
    float                 BodyAngleXY;                              // +0x24
    float                 BodyAngleYZ;                              // +0x28
    float                 HeadAngleRelToBodyXY;                     // +0x2c
    float                 HeadAngleRelToBodyYZ;                     // +0x30
    float                 EyeAnglesRelToHeadXY;                     // +0x34
    float                 EyeAnglesRelToHeadYZ;                     // +0x38
    long                  LookTimer;                                // +0x3c
    bool                  UsingOldStyleTurning;                     // +0x40
    bool                  TurningViaAnimationDisable;               // +0x41
    float                 FractionOfTurningToAcceptEachFrame;       // +0x44
    long                  HeadTurnState;                            // +0x48
    long                  BodyTurnState;                            // +0x4c
    bool                  CanTurnBody;                              // +0x50
    float                 BodyVelocityXY;                           // +0x54
    float                 BodyVelocityYZ;                           // +0x58
    float                 HeadVelocityXY;                           // +0x5c
    float                 HeadVelocityYZ;                           // +0x60
    long                  LookFlags;                                // +0x64
};

void CTCLook::FrameUpdateHeadInterested()
{
    CTCTargetingBase* targeting;
    CThing* target = 0;
    if (GetParentThing().GetTC(&targeting))
        target = targeting->GetPNearestInterestingThing();
    if (target == 0 && !targeting->IsLookingAtCamera()) {
        CentreHead();
        return;
    }
    C3DVector pos;
    if (targeting->IsLookingAtCamera()) {
        pos = GetWorld().GetGameCamera()->PeekCurrentPos();
    } else {
        CTCLook* look;
        if (target->GetTC(&look)) {
            pos = look->GetHeadPos();
        } else {
            NGraphicAppearance::CTCGraphicAppearance* appearance = target->GetTCGraphicAppearance();
            if (!(appearance && appearance->GetFocalPosIfLoaded(pos, 1.0f)))
                pos = target->GetPhysicsTC()->GetPosition();
        }
    }
    if (!(LookFlags & 0x38))
        LookFlags |= 0x10;
    FrameUpdateHeadLookAtPos(pos, false);
}