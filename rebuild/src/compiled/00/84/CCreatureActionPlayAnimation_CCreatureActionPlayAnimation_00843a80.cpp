// CCreatureAction_PlayAnimation::CCreatureAction_PlayAnimation(CThingCreatureBase& owner, const CCharString& animName, float transitionInTime, const CSoundPair& sounds, bool useAnimationMovement, bool usePhysics, bool animMayNeedCameraPositionUpdatedInSpeakAndWait, long interruptionGroupID, long interruptionPriority) -- the base action (speeds -1 / 1) on a copy of the owner's random loaded animation of that name whose transition-in time is the given seconds * 15, then the play settings: no last-frame hold, no loop, loop count 0
// Leaf: ??0CCreatureAction_PlayAnimation@@QAE@AAVCThingCreatureBase@@ABVCCharString@@MABVCSoundPair@@_N33JJ@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00843A80, 182 bytes, thiscall, `ret 24h`, returns this. Vtable 0x01273A0C.
// Identity (proven): it stores vtable 0x01273A0C (RTTI .?AVCCreatureAction_PlayAnimation@@); of the five Ego_r.pdb
//   CCreatureAction_PlayAnimation ctors only ??0..@AAVCThingCreatureBase@@ABVCCharString@@MABVCSoundPair@@_N33JJ@Z (donor
//   0x0051E1D0) takes nine arguments (`ret 24h`) with a float third (fld/fmul/_ftol2 here); its landed callers 0x0056A980 and
//   0x009574D0 bind the same (name, float, sounds, 3 x bool, 2 x long) overload.
// Callees: CCreatureActionBase::GetRandomAnimFromName 0x006924B0 (protected static __fastcall),
//   CAnimationEntry::CAnimationEntry(const CAnimationEntry&, long) 0x005DEC70 (PDB), CCreatureActionBase::
//   CCreatureActionBase(CThingCreatureBase&, float, float, const CAnimationEntry&, const CSoundPair&, long, long) 0x00693B30,
//   _ftol2; the CAnimationEntry temporary's inline destructor releases its CIVCountedPointer<CAnimComponentMap> Components
//   (CIVCountedPointeeBase slot 1 = Delete). Float pool 0x0124E640 = 15.0f.
// Layout: Ego_r.pdb CAnimationEntry (0x10: BankIndex, AnimName, GroupName, Components) and CCreatureAction_PlayAnimation
//   (UseAnimationMovement +0xA8 .. AnimMayNeedCameraPositionUpdatedInSpeakAndWait +0xB0, engine/CCreatureAction_PlayAnimation.h);
//   modelled locally because the engine headers are flat (no vptr hierarchy, no member functions).
// Shape: the landed CCreatureAction_SheatheItemToInventory ctor 0x0069EF80 (CAnimationEntry temporary built in the frame,
//   released after the base ctor, null store elided).
#define CIVCountedPointeeBase CIVCountedPointeeBasePdb
#include "engine/CIVCountedPointeeBase.h"
#undef CIVCountedPointeeBase
#define CCreatureActionBase CCreatureActionBasePdb
#include "engine/CCreatureActionBase.h"
#undef CCreatureActionBase
#define CCreatureAction_PlayAnimation CCreatureAction_PlayAnimationPdb
#include "engine/CCreatureAction_PlayAnimation.h"
#undef CCreatureAction_PlayAnimation
class CCharString;
class CThingCreatureBase;
class CSoundPair;
class CIVCountedPointeeBase {
public:
    virtual ~CIVCountedPointeeBase();
    virtual void Delete() const;                             // slot 1
    mutable unsigned long IVRefCount;                        // +0x04
    long Release() const { long result = (long)--IVRefCount; if (result == 0) Delete(); return result; }
};
class CAnimComponentMap;
template <class T> class CIVCountedPointer {
public:
    ~CIVCountedPointer() { if (Object) Object->Release(); }
    CIVCountedPointeeBase* Object;
};
class CAnimationEntry {                                      // 0x10 bytes
public:
    CAnimationEntry(const CAnimationEntry& other, long transitionInTime);   // 0x005DEC70
    long BankIndex;                                          // +0x00
    long AnimName;                                           // +0x04 CDefString
    long GroupName;                                          // +0x08 CDefString
    CIVCountedPointer<CAnimComponentMap> Components;         // +0x0C
};
class CCreatureActionBase {
public:
    CCreatureActionBase(CThingCreatureBase& creature, float a, float b, const CAnimationEntry& anim, const CSoundPair& sound,
                        long c, long d);                     // 0x00693B30
    virtual ~CCreatureActionBase();
    unsigned char _body[0xa8 - 4];                           // retail sizeof(CCreatureActionBase) 0xA8
protected:
    static const CAnimationEntry& __fastcall GetRandomAnimFromName(const CThingCreatureBase& creature,
                                                                const CCharString& name);   // 0x006924B0
};
class CCreatureAction_PlayAnimation : public CCreatureActionBase {
public:
    CCreatureAction_PlayAnimation(CThingCreatureBase& owner, const CCharString& animName, float transitionInTime,
        const CSoundPair& sounds, bool useAnimationMovement, bool usePhysics, bool animMayNeedCameraPositionUpdatedInSpeakAndWait,
        long interruptionGroupID, long interruptionPriority);
    bool UseAnimationMovement;                               // +0xA8
    bool UsePhysics;                                         // +0xA9
    bool StayOnLastFrame;                                    // +0xAA
    bool Looping;                                            // +0xAB
    long NumberOfTimesToLoop;                                // +0xAC
    bool AnimMayNeedCameraPositionUpdatedInSpeakAndWait;     // +0xB0
};

CCreatureAction_PlayAnimation::CCreatureAction_PlayAnimation(CThingCreatureBase& owner, const CCharString& animName,
        float transitionInTime, const CSoundPair& sounds, bool useAnimationMovement, bool usePhysics,
        bool animMayNeedCameraPositionUpdatedInSpeakAndWait, long interruptionGroupID, long interruptionPriority)
    : CCreatureActionBase(owner, -1.0f, 1.0f,
          CAnimationEntry(GetRandomAnimFromName(owner, animName), (long)(transitionInTime * 15.0f)), sounds,
          interruptionGroupID, interruptionPriority)
    , UseAnimationMovement(useAnimationMovement)
    , UsePhysics(usePhysics)
    , StayOnLastFrame(false)
    , Looping(false)
    , NumberOfTimesToLoop(0)
    , AnimMayNeedCameraPositionUpdatedInSpeakAndWait(animMayNeedCameraPositionUpdatedInSpeakAndWait)
{
}