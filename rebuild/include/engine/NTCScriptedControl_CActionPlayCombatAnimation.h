#ifndef FABLE_ENGINE_NTCSCRIPTEDCONTROL_CACTIONPLAYCOMBATANIMATION_H
#define FABLE_ENGINE_NTCSCRIPTEDCONTROL_CACTIONPLAYCOMBATANIMATION_H
// NTCScriptedControl::CActionPlayCombatAnimation (new, chunk 765): RTTI .?AVCActionPlayCombatAnimation@NTCScriptedControl@@ :
// CTCScriptedControl::CActionBase; retail vtable 0x0128D728: 0 scalar deleting dtor 0x009050A0, 1 Update 0x00712A80, 2 OnActivate
// 0x00712A70, 3 base OnAdd, 4 GetActionName 0x009035F0, 5 base OnCleanup. Ego_r.pdb members (sizeof 0x18): PlayedAnim +0x0C,
// UsePhysics +0x0D, Animation +0x10, AddAsQueuedAction +0x14, WaitForAnimationToFinish +0x15,
// AnimMayNeedCameraPositionUpdatedInSpeakAndWait +0x16, AllowLooking +0x17.
// Includer contract: FABLE_RETAIL_CTCBASE, FABLE_RETAIL_CTCSCRIPTEDCONTROL_ACTIONBASE, FABLE_RETAIL_CCHARSTRING and fable_string.h.
#include "engine/CTCScriptedControl.h"
namespace NTCScriptedControl {
class CActionPlayCombatAnimation : public CTCScriptedControl::CActionBase {
public:
    CActionPlayCombatAnimation(const CCharString&, bool, bool, bool, bool, bool);
    virtual ~CActionPlayCombatAnimation();
    virtual void Update();                                               //  1 0x00712A80
    virtual void OnActivate();                                           //  2 0x00712A70
    virtual CCharString GetActionName() const;                           //  4 0x009035F0
private:
    bool PlayedAnim;                                                     // +0x0c
    bool UsePhysics;                                                     // +0x0d
    CCharString Animation;                                               // +0x10
    bool AddAsQueuedAction;                                              // +0x14
    bool WaitForAnimationToFinish;                                       // +0x15
    bool AnimMayNeedCameraPositionUpdatedInSpeakAndWait;                 // +0x16
    bool AllowLooking;                                                   // +0x17
};
}
FABLE_STATIC_ASSERT(sizeof(NTCScriptedControl::CActionPlayCombatAnimation) == 0x18);
#endif
