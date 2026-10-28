// CCreatureAction_PlayAnimationWithLookTurning::FinishAction() -- the base CCreatureActionBase::FinishAction; then, unless the creature is aborting its current action, a looping animation with loops left queues itself again as a new CCreatureAction_PlayAnimationWithLookTurning on the same animation entry and settings with the loop count decremented (-1 = forever stays -1)
// Leaf: ?FinishAction@CCreatureAction_PlayAnimationWithLookTurning@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00843940, 208 bytes, thiscall, `ret`. Vtable 0x012740F4 slot 4.
// Identity (proven): RTTI vtable 0x012740F4 (.?AVCCreatureAction_PlayAnimationWithLookTurning@@) slot 4 = this body;
//   Ego_r.pdb / FableWin.pdb hold ?FinishAction@CCreatureAction_PlayAnimationWithLookTurning@@UAEXXZ (donor 0x0051E080, the
//   PlayAnimation one at 0x0051E290 follows it, as retail 0x00843940 / 0x00843B40); the local it builds is the landed
//   LookTurning ctor 0x00842DF0 (PDB ..ABVCAnimationEntry@@_N2JABVCSoundPair@@222JJ@Z). The body is byte-identical to
//   CCreatureAction_PlayAnimation::FinishAction 0x00843B40 apart from that ctor: it re-implements the override rather than
//   calling the PlayAnimation one (the base call is CCreatureActionBase::FinishAction 0x00694430).
// Callees: CCreatureActionBase::FinishAction() 0x00694430, CBaseIntelligentPointer::GetPItem() const 0x00A01B50 (x3),
//   CThingCreatureBase::IsAbortingCurrentAction() const 0x00661E70, CCreatureAction_PlayAnimationWithLookTurning::
//   CCreatureAction_PlayAnimationWithLookTurning(CThingCreatureBase&, const CAnimationEntry&, bool, bool, long,
//   const CSoundPair&, bool, bool, bool, long, long) 0x00842DF0, CThingCreatureBase::SetCurrentAction(const
//   CCreatureActionBase&) 0x006644F0, CCreatureActionBase::~CCreatureActionBase() 0x00693EF0 (the local's implicit dtor).
// Layout: as 0x00843B40 (Ego_r.pdb CCreatureActionBase / CCreatureAction_PlayAnimation members; the LookTurning class adds no
//   data). Modelled locally because the engine headers are flat (no vptr hierarchy, no member functions).
#define CCreatureActionBase CCreatureActionBasePdb
#include "engine/CCreatureActionBase.h"
#undef CCreatureActionBase
#define CCreatureAction_PlayAnimation CCreatureAction_PlayAnimationPdb
#include "engine/CCreatureAction_PlayAnimation.h"
#undef CCreatureAction_PlayAnimation
#define CCreatureAction_PlayAnimationWithLookTurning CCreatureAction_PlayAnimationWithLookTurningPdb
#include "engine/CCreatureAction_PlayAnimationWithLookTurning.h"
#undef CCreatureAction_PlayAnimationWithLookTurning
#define CThingCreatureBase CThingCreatureBasePdb
#include "engine/CThingCreatureBase.h"
#undef CThingCreatureBase
class CBaseObject;
class CAnimationEntry;
class CCreatureActionBase;
class CThingCreatureBase {
public:
    bool IsAbortingCurrentAction() const;                    // 0x00661E70
    bool SetCurrentAction(const CCreatureActionBase& action); // 0x006644F0
};
class CSoundPair {
public:
    long FirstSound;
    long LastSound;
};
class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer();                               // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                      // 0x00A01C10
    CBaseObject* GetPItem() const;                           // 0x00A01B50
    void* PData;
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* operator->() const { return (T*)GetPItem(); }
    T& operator*() const { return *(T*)GetPItem(); }
};
template <class T> class CCountedPointer {
public:
    T& operator*() const { return *Data; }
    T* Data;
    void* Info;
};
class CCreatureActionBase {
public:
    virtual ~CCreatureActionBase();                          // 0x00693EF0
    virtual void FinishAction();                             // 0x00694430
    unsigned char _pad04[4];
    CIntelligentPointer<CThingCreatureBase> PCreature;       // +0x08
    long TotalTime;                                          // +0x10
    long ActionTime;                                         // +0x14
    long AnimDelayTime;                                      // +0x18
    long FramesRunning;                                      // +0x1C
    long InterruptionGroupID;                                // +0x20
    long InterruptionPriority;                               // +0x24
    unsigned char _pad28[0x34 - 0x28];
    CSoundPair Sounds;                                       // +0x34
    unsigned char _pad3c[0x74 - 0x3c];
    CCountedPointer<CAnimationEntry> PAnimEntry;             // +0x74
    unsigned char _pad7c[0xa8 - 0x7c];
};
class CCreatureAction_PlayAnimation : public CCreatureActionBase {
public:
    CCreatureAction_PlayAnimation(CThingCreatureBase& creature, const CAnimationEntry& anim, bool stayOnLastFrame, bool looping,
        long numberOfTimesToLoop, const CSoundPair& sounds, bool useAnimationMovement, bool usePhysics,
        bool animMayNeedCameraPositionUpdatedInSpeakAndWait, long interruptionGroupID, long interruptionPriority);   // 0x008425E0
    bool UseAnimationMovement;                               // +0xA8
    bool UsePhysics;                                         // +0xA9
    bool StayOnLastFrame;                                    // +0xAA
    bool Looping;                                            // +0xAB
    long NumberOfTimesToLoop;                                // +0xAC
    bool AnimMayNeedCameraPositionUpdatedInSpeakAndWait;     // +0xB0
};

class CCreatureAction_PlayAnimationWithLookTurning : public CCreatureAction_PlayAnimation {   // adds no data
public:
    CCreatureAction_PlayAnimationWithLookTurning(CThingCreatureBase& creature, const CAnimationEntry& anim, bool stayOnLastFrame,
        bool looping, long numberOfTimesToLoop, const CSoundPair& sounds, bool useAnimationMovement, bool usePhysics,
        bool animMayNeedCameraPositionUpdatedInSpeakAndWait, long interruptionGroupID, long interruptionPriority);   // 0x00842DF0
    virtual void FinishAction();
};

void CCreatureAction_PlayAnimationWithLookTurning::FinishAction()
{
    CCreatureActionBase::FinishAction();
    if (PCreature->IsAbortingCurrentAction())
        return;
    if (Looping && NumberOfTimesToLoop != 0) {
        long loops;
        if (NumberOfTimesToLoop == -1)
            loops = -1;
        else
            loops = NumberOfTimesToLoop - 1;
        PCreature->SetCurrentAction(CCreatureAction_PlayAnimationWithLookTurning(*PCreature, *PAnimEntry, StayOnLastFrame, Looping, loops,
            Sounds, UseAnimationMovement, UsePhysics, AnimMayNeedCameraPositionUpdatedInSpeakAndWait, InterruptionGroupID,
            InterruptionPriority));
    }
}