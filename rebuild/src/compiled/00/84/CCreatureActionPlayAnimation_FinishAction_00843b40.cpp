// CCreatureAction_PlayAnimation::FinishAction() -- the base FinishAction; then, unless the creature is aborting its current action, a looping animation with loops left queues itself again: a new CCreatureAction_PlayAnimation on the same animation entry and settings with the loop count decremented (-1 = forever stays -1)
// Leaf: ?FinishAction@CCreatureAction_PlayAnimation@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00843B40, 208 bytes, thiscall, `ret`. Vtable 0x01273A0C slot 4.
// Identity (proven): RTTI vtable 0x01273A0C (.?AVCCreatureAction_PlayAnimation@@) slot 4 = this body (slot 4 is
//   FinishAction in every CCreatureActionBase vtable, e.g. the landed 0x00843C10 PlayIntoLoopOutOfAnimation); Ego_r.pdb /
//   FableWin.pdb hold ?FinishAction@CCreatureAction_PlayAnimation@@UAEXXZ; the local it builds is the full
//   CCreatureAction_PlayAnimation ctor 0x008425E0 (11 arguments, `_N2JABVCSoundPair@@222JJ`), whose landed caller 0x00842DF0
//   binds the PDB spelling used here.
// Callees: CCreatureActionBase::FinishAction() 0x00694430, CBaseIntelligentPointer::GetPItem() const 0x00A01B50 (x3),
//   CThingCreatureBase::IsAbortingCurrentAction() const 0x00661E70 (bit 2 of byte +0x15D; Ego_r donor order
//   IsAbortingCurrentAction then IsInMelee = retail 0x00661E70 / 0x00661E80), CCreatureAction_PlayAnimation::
//   CCreatureAction_PlayAnimation(CThingCreatureBase&, const CAnimationEntry&, bool, bool, long, const CSoundPair&, bool, bool,
//   bool, long, long) 0x008425E0, CThingCreatureBase::SetCurrentAction(const CCreatureActionBase&) 0x006644F0,
//   CCreatureActionBase::~CCreatureActionBase() 0x00693EF0 (the local's implicit destructor).
// Layout: Ego_r.pdb CCreatureActionBase (PCreature +0x08, InterruptionGroupID +0x20, InterruptionPriority +0x24, Sounds
//   +0x34, PAnimEntry +0x74; the 0xC container shrink comes after +0x84) and CCreatureAction_PlayAnimation (UseAnimationMovement
//   +0xA8, UsePhysics +0xA9, StayOnLastFrame +0xAA, Looping +0xAB, NumberOfTimesToLoop +0xAC,
//   AnimMayNeedCameraPositionUpdatedInSpeakAndWait +0xB0), as engine/CCreatureActionBase.h and engine/
//   CCreatureAction_PlayAnimation.h give them; the classes are modelled locally because those headers are flat (no vptr
//   hierarchy, no member functions).
#define CCreatureActionBase CCreatureActionBasePdb
#include "engine/CCreatureActionBase.h"
#undef CCreatureActionBase
#define CCreatureAction_PlayAnimation CCreatureAction_PlayAnimationPdb
#include "engine/CCreatureAction_PlayAnimation.h"
#undef CCreatureAction_PlayAnimation
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
    virtual void FinishAction();
    bool UseAnimationMovement;                               // +0xA8
    bool UsePhysics;                                         // +0xA9
    bool StayOnLastFrame;                                    // +0xAA
    bool Looping;                                            // +0xAB
    long NumberOfTimesToLoop;                                // +0xAC
    bool AnimMayNeedCameraPositionUpdatedInSpeakAndWait;     // +0xB0
};

void CCreatureAction_PlayAnimation::FinishAction()
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
        PCreature->SetCurrentAction(CCreatureAction_PlayAnimation(*PCreature, *PAnimEntry, StayOnLastFrame, Looping, loops,
            Sounds, UseAnimationMovement, UsePhysics, AnimMayNeedCameraPositionUpdatedInSpeakAndWait, InterruptionGroupID,
            InterruptionPriority));
    }
}