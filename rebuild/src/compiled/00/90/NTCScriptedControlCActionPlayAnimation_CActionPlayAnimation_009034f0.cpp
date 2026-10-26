// NTCScriptedControl::CActionPlayAnimation::CActionPlayAnimation(const CCharString&, bool, bool, long, bool, long, bool, bool, bool, bool, bool) -- scripted "play animation" action
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009034F0, length 117 bytes (`ret 2Ch`).
// Callees: CBase::CBase @ 0x0099A2F0, CCharString::CCharString(const CCharString&) @ 0x0099EC30 (out of line,
// so CCharString is modelled locally). Vtable 0x0128D6F4 = NTCScriptedControl::CActionPlayAnimation
// (rtti-vtables.tsv).
//
// Shape: the landed scripted-action shape (0x00716790): a `__declspec(novtable)` CActionBase intermediate whose
// init list zeroes PTCScript (+0x04), Activated (+0x08), Terminated (+0x09) from one `xor eax,eax`, the
// derived PlayedAnim (+0x0c) zeroed with them before the vtable store, the Animation string copy-constructed
// after it, then the ten scalar arguments stored in member order (VC interleaves the loads). The argument
// order differs from the member order: UsePhysics (+0x1d) is the 8th scalar argument (PDB member names).
// Leaf: ??0CActionPlayAnimation

class CCharString { public:
    void* m_pData;
    CCharString(const CCharString& other);      // 0x0099EC30
};

struct CBase {
    virtual void v00();
    CBase();                                    // 0x0099A2F0
};

namespace NTCScriptedControl {

struct __declspec(novtable) CActionBase : CBase {   // abstract: its own vtable store is dead
    void* PTCScript;                            // +0x04 CTCScriptedControl*
    bool  Activated;                            // +0x08
    bool  Terminated;                           // +0x09
    CActionBase() : CBase(), PTCScript(0), Activated(false), Terminated(false) { }
};

struct CActionPlayAnimation : CActionBase {     // vtable 0x0128D6F4
    bool        PlayedAnim;                     // +0x0c
    CCharString Animation;                      // +0x10
    bool        StayOnLastFrame;                // +0x14
    bool        Looping;                        // +0x15
    long        NumLoops;                       // +0x18
    bool        UseMovement;                    // +0x1c
    bool        UsePhysics;                     // +0x1d
    long        Priority;                       // +0x20
    bool        AddAsQueuedAction;              // +0x24
    bool        WaitForAnimationToFinish;       // +0x25
    bool        AnimMayNeedCameraPositionUpdatedInSpeakAndWait;   // +0x26
    bool        AllowLooking;                   // +0x27
    virtual void v00();
    CActionPlayAnimation(const CCharString& animation, bool stayOnLastFrame, bool looping, long numLoops,
                         bool useMovement, long priority, bool addAsQueuedAction, bool waitForAnimationToFinish,
                         bool usePhysics, bool animMayNeedCameraPositionUpdated, bool allowLooking);
};

CActionPlayAnimation::CActionPlayAnimation(const CCharString& animation, bool stayOnLastFrame, bool looping,
                                           long numLoops, bool useMovement, long priority, bool addAsQueuedAction,
                                           bool waitForAnimationToFinish, bool usePhysics,
                                           bool animMayNeedCameraPositionUpdated, bool allowLooking)
    : CActionBase()
    , PlayedAnim(false)
    , Animation(animation)
    , StayOnLastFrame(stayOnLastFrame)
    , Looping(looping)
    , NumLoops(numLoops)
    , UseMovement(useMovement)
    , UsePhysics(usePhysics)
    , Priority(priority)
    , AddAsQueuedAction(addAsQueuedAction)
    , WaitForAnimationToFinish(waitForAnimationToFinish)
    , AnimMayNeedCameraPositionUpdatedInSpeakAndWait(animMayNeedCameraPositionUpdated)
    , AllowLooking(allowLooking)
{
}

}