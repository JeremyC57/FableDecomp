// CCreatureAction_PlayAnimation::CCreatureAction_PlayAnimation(CThingCreatureBase*, const CCharString&, const CSoundPair&, bool, bool, bool, long, long) -- base CCreatureActionBase(owner, -1.0, 1.0, the owner's animation of that name, sound, c, d), then the play flags (not looping)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008424F0, length 116 bytes (+ int3 padding). Vtable 0x01273A0C.
// Callees: the creature's animation lookup by name @ 0x006924B0 (`__fastcall`: ecx owner, edx the name reference; the landed
// 0x00842A20 calls it with a kind word), CCreatureActionBase::CCreatureActionBase(CThingCreatureBase*, float, float, CAnimationEntry*,
// const CSoundPair&, long, long) @ 0x00693B30.
// Shape: `ret 20h`; arguments evaluated right to left (d, c, the sound reference, the lookup, 1.0f, -1.0f, the owner); the members are
// initialiser-list entries in declaration order (StayOnLastFrame, Looping and NumberOfTimesToLoop share the zero register).
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
CAnimationEntry* __fastcall GetAnimationEntry(CThingCreatureBase* owner, const CCharString& name);   // 0x006924B0

struct CCreatureAction_PlayAnimation : CCreatureActionBase {   // vtable 0x01273A0C; members = engine header names (PDB +0xb4.. sit 0xc high)
    bool UseAnimationMovement;              // +0xa8
    bool UsePhysics;                        // +0xa9
    bool StayOnLastFrame;                   // +0xaa
    bool Looping;                           // +0xab
    long NumberOfTimesToLoop;               // +0xac
    bool AnimMayNeedCameraPositionUpdatedInSpeakAndWait;   // +0xb0
    virtual void v00();
    CCreatureAction_PlayAnimation(CThingCreatureBase* owner, const CCharString& animationName, const CSoundPair& sound,
                                  bool useAnimationMovement, bool usePhysics, bool cameraUpdate, long c, long d);
};

CCreatureAction_PlayAnimation::CCreatureAction_PlayAnimation(CThingCreatureBase* owner, const CCharString& animationName, const CSoundPair& sound,
                                                             bool useAnimationMovement, bool usePhysics, bool cameraUpdate, long c, long d)
    : CCreatureActionBase(owner, -1.0f, 1.0f, GetAnimationEntry(owner, animationName), sound, c, d)
    , UseAnimationMovement(useAnimationMovement)
    , UsePhysics(usePhysics)
    , StayOnLastFrame(false)
    , Looping(false)
    , NumberOfTimesToLoop(0)
    , AnimMayNeedCameraPositionUpdatedInSpeakAndWait(cameraUpdate)
{
}
