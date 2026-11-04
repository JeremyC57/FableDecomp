#ifndef FABLE_ENGINE_NTCSCRIPTEDCONTROL_CACTIONPLAYANIMATION_H
#define FABLE_ENGINE_NTCSCRIPTEDCONTROL_CACTIONPLAYANIMATION_H
// NTCScriptedControl::CActionPlayAnimation (new, chunk 765): RTTI .?AVCActionPlayAnimation@NTCScriptedControl@@ : CTCScriptedControl::
// CActionBase; retail vtable 0x0128D6F4: 0 scalar deleting dtor 0x00905070, 1 Update 0x00712900, 2 OnActivate 0x007128F0, 3 base OnAdd,
// 4 GetActionName 0x00903570, 5 base OnCleanup. Ego_r.pdb members (sizeof 0x28): PlayedAnim +0x0C, Animation +0x10, StayOnLastFrame
// +0x14, Looping +0x15, NumLoops +0x18, UseMovement +0x1C, UsePhysics +0x1D, Priority +0x20, AddAsQueuedAction +0x24,
// WaitForAnimationToFinish +0x25, AnimMayNeedCameraPositionUpdatedInSpeakAndWait +0x26, AllowLooking +0x27.
// Includer contract: FABLE_RETAIL_CTCBASE, FABLE_RETAIL_CTCSCRIPTEDCONTROL_ACTIONBASE, FABLE_RETAIL_CCHARSTRING and fable_string.h.
#include "engine/CTCScriptedControl.h"
namespace NTCScriptedControl {
class CActionPlayAnimation : public CTCScriptedControl::CActionBase {
public:
    CActionPlayAnimation(const CCharString&, bool, bool, long, bool, long, bool, bool, bool, bool, bool);
    virtual ~CActionPlayAnimation();
    virtual void Update();                                               //  1 0x00712900
    virtual void OnActivate();                                           //  2 0x007128F0
    virtual CCharString GetActionName() const;                           //  4 0x00903570
private:
    bool PlayedAnim;                                                     // +0x0c
    CCharString Animation;                                               // +0x10
    bool StayOnLastFrame;                                                // +0x14
    bool Looping;                                                        // +0x15
    long NumLoops;                                                       // +0x18
    bool UseMovement;                                                    // +0x1c
    bool UsePhysics;                                                     // +0x1d
    long Priority;                                                       // +0x20
    bool AddAsQueuedAction;                                              // +0x24
    bool WaitForAnimationToFinish;                                       // +0x25
    bool AnimMayNeedCameraPositionUpdatedInSpeakAndWait;                 // +0x26
    bool AllowLooking;                                                   // +0x27
};
}
FABLE_STATIC_ASSERT(sizeof(NTCScriptedControl::CActionPlayAnimation) == 0x28);
#endif
