// CCreatureAction_PlayAnimationWithLookTurning::CCreatureAction_PlayAnimationWithLookTurning(CThingCreatureBase&, const CAnimationEntry&, const CSoundPair&, bool, bool, long, long) -- the play-animation-entry base with the camera-update flag cleared, then the look-turning vtable
// Leaf: ??0CCreatureAction_PlayAnimationWithLookTurning@@QAE@AAVCThingCreatureBase@@ABVCAnimationEntry@@ABVCSoundPair@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00842E40, 58 bytes, `ret 1Ch`, returns this.
// Identity: ??0CCreatureAction_PlayAnimationWithLookTurning@@QAE@AAVCThingCreatureBase@@ABVCAnimationEntry@@ABVCSoundPair@@_N3JJ@Z (Ego_r.pdb); the own vtable stored after the base constructor is 0x012740F4 =
//   CCreatureAction_PlayAnimationWithLookTurning (RTTI). The manifest label "CCreatureAction_PlayAnimation_ConstructorWithEntry" is descriptive.
// Callee: CCreatureAction_PlayAnimation::CCreatureAction_PlayAnimation 0x00842570 (landed; the animation-entry overload).
// Layout: the class adds no data to its 0xB4-byte base (the landed Clone 0x0084C4E0 model).
// Shape: an init-list base call with the arguments passed straight through (re-read from the stack right to left), then
//   the derived vtable store; the PDB reference parameters.
class CThingCreatureBase;
class CAnimationEntry;
class CCharString;
class CSoundPair;

class CCreatureAction_PlayAnimation {                    // retail sizeof 0xb4 (vtable 0x01273A0C)
public:
    virtual void v00();
    CCreatureAction_PlayAnimation(CThingCreatureBase& owner, const CCharString& animName, const CSoundPair& sound,
                                  bool useAnimationMovement, bool usePhysics, bool cameraUpdate, long c, long d);       // 0x008424F0
    CCreatureAction_PlayAnimation(CThingCreatureBase& owner, const CAnimationEntry& anim, const CSoundPair& sound,
                                  bool useAnimationMovement, bool usePhysics, bool cameraUpdate, long c, long d);       // 0x00842570
    CCreatureAction_PlayAnimation(CThingCreatureBase& owner, const CAnimationEntry& anim, bool stayOnLastFrame, bool looping,
                                  long numberOfTimesToLoop, const CSoundPair& sound, bool useAnimationMovement, bool usePhysics,
                                  bool cameraUpdate, long c, long d);                                                     // 0x008425E0
private:
    unsigned char _body[0xb4 - 4];
};

class CCreatureAction_PlayAnimationWithLookTurning : public CCreatureAction_PlayAnimation {   // vtable 0x012740F4, adds no data
public:
    virtual void v00();
    CCreatureAction_PlayAnimationWithLookTurning(CThingCreatureBase& owner, const CAnimationEntry& anim, const CSoundPair& sound,
                                                 bool useAnimationMovement, bool usePhysics, long c, long d);
};

CCreatureAction_PlayAnimationWithLookTurning::CCreatureAction_PlayAnimationWithLookTurning(CThingCreatureBase& owner,
        const CAnimationEntry& anim, const CSoundPair& sound, bool useAnimationMovement, bool usePhysics, long c, long d)
    : CCreatureAction_PlayAnimation(owner, anim, sound, useAnimationMovement, usePhysics, false, c, d)
{
}