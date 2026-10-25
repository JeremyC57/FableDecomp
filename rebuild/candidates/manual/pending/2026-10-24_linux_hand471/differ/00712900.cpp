// NTCScriptedControl::CActionPlayAnimation::Update() -- unless the creature is incapacitated: once the animation was handed over, finish when the creature is idle; otherwise build a play-animation creature action (with look turning when allowed) from a random animation of the given name, queue it or make it current, finish at once unless told to wait for it, and free the local action
// Grade: DIFFER(362v362) -- one register choice: the played-path creature read is `mov eax,[esi+4] ; mov ecx,[eax+4]` here, retail `mov ecx,[esi+4] ; mov ecx,[ecx+4]`; next: write that one test as `PTCScript->ParentThing->IsPerformingAnyAction()` (the first probe's direct form produced the ecx/ecx pair there)
// Leaf: ?Update@CActionPlayAnimation@NTCScriptedControl@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00712900, 362 bytes (+ int3 padding), plain `ret`. Slot 1 of vtable 0x0128D6F4
//   (rtti-vtables.tsv); Ego_r.pdb ?Update@CActionPlayAnimation@NTCScriptedControl@@UAEXXZ.
// Callees: IsIncapacitated @ 0x0085F0C0 (__fastcall, the creature in ecx), CThingCreatureBase::IsPerformingAnyAction
//   @ 0x006622A0 / GetRandomAnimFromName(const CCharString&, long) const @ 0x00662D80 / AddQueuedAction @ 0x00664490
//   (landed) / SetCurrentAction(const CCreatureActionBase&) @ 0x006644F0, operator new @ 0x00BFEA1A (0xB4 bytes),
//   CCreatureAction_PlayAnimationWithLookTurning @ 0x00842DF0 and CCreatureAction_PlayAnimation @ 0x008425E0 (both the
//   Ego_r.pdb (creature&, const CAnimationEntry&, bool, bool, long, const CSoundPair&, bool, bool, bool, long, long)
//   overload), the action's virtual deleting destructor (slot 0).
// Layout: Ego_r.pdb CActionPlayAnimation = retail (the landed constructor 0x009034F0): PlayedAnim +0x0C, Animation
//   +0x10, StayOnLastFrame +0x14, Looping +0x15, NumLoops +0x18, UseMovement +0x1C, UsePhysics +0x1D, Priority +0x20,
//   AddAsQueuedAction +0x24, WaitForAnimationToFinish +0x25, AnimMayNeedCameraPositionUpdatedInSpeakAndWait +0x26,
//   AllowLooking +0x27; CActionBase PTCScript +0x04, Terminated +0x09.
// Shape: the creature comes from an inline GetCreature() accessor, re-read at every use: written twice in each
//   new-expression (constructor argument and `this` of the animation lookup) VC7.1 reads it once into edi; the empty
//   CSoundPair is an 8-byte frame temporary (the `sub esp,8`); the not-yet-played case is the if-arm (the played
//   case's idle test sits after the shared epilogue); the queue / set-current choice is a nested if whose two
//   SetCurrentAction arms VC tail-merges (the creature read for either arm hoisted above the AddAsQueuedAction test).
class CCharString {
public:
    void* PStringData;
};
class CAnimationEntry;
struct CSoundPair {
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) {}
};

class CCreatureActionBase {
public:
    virtual ~CCreatureActionBase();
};
class CThingCreatureBase {
public:
    bool IsPerformingAnyAction();                                   // 0x006622A0
    const CAnimationEntry& GetRandomAnimFromName(const CCharString& name, long index) const;   // 0x00662D80
    void AddQueuedAction(const CCreatureActionBase& action);        // 0x00664490
    bool SetCurrentAction(const CCreatureActionBase& action);       // 0x006644F0
};
bool __fastcall IsIncapacitated(CThingCreatureBase* creature);      // 0x0085F0C0

class CCreatureAction_PlayAnimation : public CCreatureActionBase {
public:
    CCreatureAction_PlayAnimation(CThingCreatureBase& creature, const CAnimationEntry& anim, bool stayOnLastFrame,
                                  bool looping, long numLoops, const CSoundPair& sounds, bool useMovement,
                                  bool usePhysics, bool animMayNeedCameraPositionUpdated, long arg10, long priority);   // 0x008425E0
    unsigned char _pad04[0xb4 - 4];
};
class CCreatureAction_PlayAnimationWithLookTurning : public CCreatureActionBase {
public:
    CCreatureAction_PlayAnimationWithLookTurning(CThingCreatureBase& creature, const CAnimationEntry& anim,
                                  bool stayOnLastFrame, bool looping, long numLoops, const CSoundPair& sounds,
                                  bool useMovement, bool usePhysics, bool animMayNeedCameraPositionUpdated, long arg10,
                                  long priority);                   // 0x00842DF0
    unsigned char _pad04[0xb4 - 4];
};

struct CTCScriptedControl {
    void*               __vftable;
    CThingCreatureBase* ParentThing;                                // +0x04
};

namespace NTCScriptedControl {

class CActionBase {
public:
    virtual ~CActionBase();
    virtual void Update();                                          // slot 1
    CTCScriptedControl* PTCScript;                                  // +0x04
    bool                Activated;                                  // +0x08
    bool                Terminated;                                 // +0x09
};

class CActionPlayAnimation : public CActionBase {
public:
    virtual void Update();
    CThingCreatureBase* GetCreature() const { return PTCScript->ParentThing; }
    bool        PlayedAnim;                                         // +0x0c
    CCharString Animation;                                          // +0x10
    bool        StayOnLastFrame;                                    // +0x14
    bool        Looping;                                            // +0x15
    long        NumLoops;                                           // +0x18
    bool        UseMovement;                                        // +0x1c
    bool        UsePhysics;                                         // +0x1d
    long        Priority;                                           // +0x20
    bool        AddAsQueuedAction;                                  // +0x24
    bool        WaitForAnimationToFinish;                           // +0x25
    bool        AnimMayNeedCameraPositionUpdatedInSpeakAndWait;     // +0x26
    bool        AllowLooking;                                       // +0x27
};

void CActionPlayAnimation::Update()
{
    if (IsIncapacitated(GetCreature()))
        return;
    if (!PlayedAnim) {
        CCreatureActionBase* action;
        if (AllowLooking)
            action = new CCreatureAction_PlayAnimationWithLookTurning(*GetCreature(),
                GetCreature()->GetRandomAnimFromName(Animation, -1), StayOnLastFrame, Looping, NumLoops,
                CSoundPair(), UseMovement, UsePhysics, AnimMayNeedCameraPositionUpdatedInSpeakAndWait, 0, Priority);
        else
            action = new CCreatureAction_PlayAnimation(*GetCreature(),
                GetCreature()->GetRandomAnimFromName(Animation, -1), StayOnLastFrame, Looping, NumLoops,
                CSoundPair(), UseMovement, UsePhysics, AnimMayNeedCameraPositionUpdatedInSpeakAndWait, 0, Priority);
        if (AddAsQueuedAction) {
            if (GetCreature()->IsPerformingAnyAction()) {
                GetCreature()->AddQueuedAction(*action);
                PlayedAnim = true;
            } else
                PlayedAnim = GetCreature()->SetCurrentAction(*action);
        } else
            PlayedAnim = GetCreature()->SetCurrentAction(*action);
        if (!WaitForAnimationToFinish)
            Terminated = true;
        delete action;
    } else {
        if (!GetCreature()->IsPerformingAnyAction())
            Terminated = true;
    }
}
}
