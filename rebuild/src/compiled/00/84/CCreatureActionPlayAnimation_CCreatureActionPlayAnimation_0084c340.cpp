// CCreatureAction_PlayAnimation::CCreatureAction_PlayAnimation(const CCreatureAction_PlayAnimation&) -- copy constructor: base copy, then every play setting copied member by member
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0084C340, length 99 bytes (+ int3 padding). Vtable 0x01273A0C.
// Callees: CCreatureActionBase::CCreatureActionBase(const CCreatureActionBase&) @ 0x0062EBC0 (out-of-line base copy constructor).
// Shape: `ret 4`; the source rides in edi; the vtable store follows the base copy, then the six members in declaration order
// (bytes through al/cl/dl, the loop count as a dword) -- the explicit memberwise copy constructor (used by Clone).
class CCreatureActionBase { public:                // retail sizeof 0xa8
    virtual void v00();
    unsigned char _body[0xa8 - 4];
    CCreatureActionBase(const CCreatureActionBase& other);   // 0x0062EBC0
};

class CCreatureAction_PlayAnimation : public CCreatureActionBase { public:   // vtable 0x01273A0C; members = engine header names (PDB +0xb4.. sit 0xc high)
    bool UseAnimationMovement;              // +0xa8
    bool UsePhysics;                        // +0xa9
    bool StayOnLastFrame;                   // +0xaa
    bool Looping;                           // +0xab
    long NumberOfTimesToLoop;               // +0xac
    bool AnimMayNeedCameraPositionUpdatedInSpeakAndWait;   // +0xb0
    virtual void v00();
    CCreatureAction_PlayAnimation(const CCreatureAction_PlayAnimation& other);
};

CCreatureAction_PlayAnimation::CCreatureAction_PlayAnimation(const CCreatureAction_PlayAnimation& other)
    : CCreatureActionBase(other)
    , UseAnimationMovement(other.UseAnimationMovement)
    , UsePhysics(other.UsePhysics)
    , StayOnLastFrame(other.StayOnLastFrame)
    , Looping(other.Looping)
    , NumberOfTimesToLoop(other.NumberOfTimesToLoop)
    , AnimMayNeedCameraPositionUpdatedInSpeakAndWait(other.AnimMayNeedCameraPositionUpdatedInSpeakAndWait)
{
}