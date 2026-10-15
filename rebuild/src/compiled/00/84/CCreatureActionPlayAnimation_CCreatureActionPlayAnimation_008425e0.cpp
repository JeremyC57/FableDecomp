// CCreatureAction_PlayAnimation::CCreatureAction_PlayAnimation(CThingCreatureBase*, CAnimationEntry*, bool, bool, long, const CSoundPair&, bool, bool, bool, long, long) -- base CCreatureActionBase(owner, -1.0, 1.0, animation, sound, c, d), then every play setting from the arguments
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008425E0, length 117 bytes. Vtable 0x01273A0C.
// Callees: CCreatureActionBase::CCreatureActionBase(CThingCreatureBase*, float, float, CAnimationEntry*, const CSoundPair&, long, long) @ 0x00693B30.
// Shape: the full overload (`ret 2Ch`): stay-on-last-frame, looping and the loop count come third to fifth, the sound sixth, the
// movement/physics/camera flags seventh to ninth; initialiser-list members in declaration order.
struct CThingCreatureBase;
struct CThing;
struct CAnimationEntry;
struct CCharString;
struct CSoundPair {                         // two sound ids, zeroed by the inline constructor (register-zeroed stack stores)
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};
struct CCreatureActionBase {                // retail sizeof 0xa8
    virtual void v00();
    unsigned char _body[0xa8 - 4];
    CCreatureActionBase(CThingCreatureBase* owner, float a, float b, CAnimationEntry* animation, const CSoundPair& sound, long c, long d);   // 0x00693B30
};

struct CCreatureAction_PlayAnimation : CCreatureActionBase {   // vtable 0x01273A0C; members = engine header names (PDB +0xb4.. sit 0xc high)
    bool UseAnimationMovement;              // +0xa8
    bool UsePhysics;                        // +0xa9
    bool StayOnLastFrame;                   // +0xaa
    bool Looping;                           // +0xab
    long NumberOfTimesToLoop;               // +0xac
    bool AnimMayNeedCameraPositionUpdatedInSpeakAndWait;   // +0xb0
    virtual void v00();
    CCreatureAction_PlayAnimation(CThingCreatureBase* owner, CAnimationEntry* animation, bool stayOnLastFrame, bool looping, long numberOfTimesToLoop,
                                  const CSoundPair& sound, bool useAnimationMovement, bool usePhysics, bool cameraUpdate, long c, long d);
};

CCreatureAction_PlayAnimation::CCreatureAction_PlayAnimation(CThingCreatureBase* owner, CAnimationEntry* animation, bool stayOnLastFrame, bool looping, long numberOfTimesToLoop,
                                                             const CSoundPair& sound, bool useAnimationMovement, bool usePhysics, bool cameraUpdate, long c, long d)
    : CCreatureActionBase(owner, -1.0f, 1.0f, animation, sound, c, d)
    , UseAnimationMovement(useAnimationMovement)
    , UsePhysics(usePhysics)
    , StayOnLastFrame(stayOnLastFrame)
    , Looping(looping)
    , NumberOfTimesToLoop(numberOfTimesToLoop)
    , AnimMayNeedCameraPositionUpdatedInSpeakAndWait(cameraUpdate)
{
}