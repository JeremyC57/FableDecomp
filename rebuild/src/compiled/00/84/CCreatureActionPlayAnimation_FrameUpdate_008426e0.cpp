// CCreatureAction_PlayAnimation::FrameUpdate() -- an animation that stays on its last frame holds its frame counter one short of the total time, then the base action frame update runs
// Leaf: ?FrameUpdate@CCreatureAction_PlayAnimation@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008426E0, 34 bytes, thiscall, tail jump to the base FrameUpdate.
// Identity (proven): RTTI vtable 0x01273A0C (.?AVCCreatureAction_PlayAnimation@@) slot 56 and the derived
//   CCreatureAction_PlayAnimationWithLookTurning vtable 0x012740F4 slot 56 (inherited) are the only references; slot 56 is
//   FrameUpdate in the action family (landed 0x008C3F20 / 0x00859D80); Ego_r.pdb / FableWin.pdb hold
//   ?FrameUpdate@CCreatureAction_PlayAnimation@@UAEXXZ. The catalog label is right.
// Callee: CCreatureActionBase::FrameUpdate() 0x00694020 (Ego_r ?FrameUpdate@CCreatureActionBase@@MAEXXZ, protected virtual;
//   called qualified, so a direct tail jump).
// Layout: engine/CCreatureActionBase.h (TotalTime +0x10, FramesRunning +0x1C) and engine/CCreatureAction_PlayAnimation.h
//   (StayOnLastFrame +0xAA), restated as a polymorphic model because those generated headers are flat (no vptr hierarchy, no
//   member functions); the same layout-rename pattern as the landed CCreatureAction_PlayAnimation::FinishAction 0x00843B40.
#define CCreatureActionBase CCreatureActionBasePdb
#include "engine/CCreatureActionBase.h"
#undef CCreatureActionBase
#define CCreatureAction_PlayAnimation CCreatureAction_PlayAnimationPdb
#include "engine/CCreatureAction_PlayAnimation.h"
#undef CCreatureAction_PlayAnimation

class CCreatureActionBase {
public:
    virtual ~CCreatureActionBase();                          // 0x00693EF0
protected:
    virtual void FrameUpdate();                              // 0x00694020
public:
    unsigned char _pad04[0x10 - 0x04];
    long TotalTime;                                          // +0x10
    long ActionTime;                                         // +0x14
    long AnimDelayTime;                                      // +0x18
    long FramesRunning;                                      // +0x1C
    unsigned char _pad20[0xa8 - 0x20];
};

class CCreatureAction_PlayAnimation : public CCreatureActionBase {
public:
    virtual void FrameUpdate();
    bool UseAnimationMovement;                               // +0xA8
    bool UsePhysics;                                         // +0xA9
    bool StayOnLastFrame;                                    // +0xAA
    bool Looping;                                            // +0xAB
    long NumberOfTimesToLoop;                                // +0xAC
    bool AnimMayNeedCameraPositionUpdatedInSpeakAndWait;     // +0xB0
};

void CCreatureAction_PlayAnimation::FrameUpdate()
{
    if (StayOnLastFrame) {
        if (FramesRunning + 1 >= TotalTime)
            FramesRunning--;
    }
    CCreatureActionBase::FrameUpdate();
}