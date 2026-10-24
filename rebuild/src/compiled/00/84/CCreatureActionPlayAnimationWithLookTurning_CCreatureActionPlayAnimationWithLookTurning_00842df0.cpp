// CCreatureAction_PlayAnimationWithLookTurning::CCreatureAction_PlayAnimationWithLookTurning(CThingCreatureBase&, const CAnimationEntry&, bool, bool, long, const CSoundPair&, bool, bool, bool, long, long) -- forwards every play setting to the full play-animation base, then the look-turning vtable
// Leaf: ??0CCreatureAction_PlayAnimationWithLookTurning@@QAE@AAVCThingCreatureBase@@ABVCAnimationEntry@@_N
// Retail: Fable.exe (TLC, VC7.1) @ 0x00842DF0, 77 bytes, `ret 2Ch`, returns this.
// Identity: ??0CCreatureAction_PlayAnimationWithLookTurning@@QAE@AAVCThingCreatureBase@@ABVCAnimationEntry@@_N2JABVCSoundPair@@222JJ@Z (Ego_r.pdb); the own vtable stored after the base constructor is 0x012740F4 =
//   CCreatureAction_PlayAnimationWithLookTurning (RTTI). The manifest label "CCreatureAction_PlayAnimationWithLookTurning_Construct" is descriptive.
// Callee: CCreatureAction_PlayAnimation::CCreatureAction_PlayAnimation 0x008425E0 (landed; the full overload).
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
    CCreatureAction_PlayAnimationWithLookTurning(CThingCreatureBase& owner, const CAnimationEntry& anim, bool stayOnLastFrame,
                                                 bool looping, long numberOfTimesToLoop, const CSoundPair& sound,
                                                 bool useAnimationMovement, bool usePhysics, bool cameraUpdate, long c, long d);
};

CCreatureAction_PlayAnimationWithLookTurning::CCreatureAction_PlayAnimationWithLookTurning(CThingCreatureBase& owner,
        const CAnimationEntry& anim, bool stayOnLastFrame, bool looping, long numberOfTimesToLoop, const CSoundPair& sound,
        bool useAnimationMovement, bool usePhysics, bool cameraUpdate, long c, long d)
    : CCreatureAction_PlayAnimation(owner, anim, stayOnLastFrame, looping, numberOfTimesToLoop, sound, useAnimationMovement,
                                    usePhysics, cameraUpdate, c, d)
{
}