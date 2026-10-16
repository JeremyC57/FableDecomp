// Grade: DIFFER(216v216) -- register choice only: retail releases through edx (mov edx,[edi] ; call [edx]) and reads the idle-path creature through eax; ours eax / ecx | tried: long usePhysics (movzx), bool usePhysics (xor+mov cl fixed), early-return tail
// NTCScriptedControl::CActionPlayCombatAnimation::Update() -- first time: build the creature action that plays the
// animation (a looking variant when AllowLooking), flag it as script-driven, queue it or make it current, finish at
// once unless WaitForAnimationToFinish, and drop this reference; afterwards finish once the creature is idle.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00712A80, length 213 bytes (+ int3 padding).
// Callees: operator new @ 0x00BFEA1A (0x118 bytes, twice), the looking variant's ctor @ 0x008B80E0 (manifest
// "CEngineClothSimulation2DAnimatedMesh" is propagated) and CCreatureAction_PlayCombatAnimation::
// CCreatureAction_PlayCombatAnimation(CThingCreatureBase*, const CCharString&, bool) @ 0x008B7FF0,
// CThingCreatureBase::IsPerformingAnyAction @ 0x006622A0 (twice), AddQueuedAction @ 0x00664490 (manifest
// "PhysicsSegment_AssignSegment"), SetCurrentAction @ 0x006644F0, the action's virtual slot 0 (reference release, 1).
// Ego_r.pdb CActionPlayCombatAnimation (0x18): PlayedAnim +0x0c, UsePhysics +0x0d, Animation +0x10,
// AddAsQueuedAction +0x14, WaitForAnimationToFinish +0x15, AnimMayNeedCameraPositionUpdatedInSpeakAndWait +0x16,
// AllowLooking +0x17; the CAction base's Terminated flag is +9, PTCScript +4 (its parent creature at +4).
// Shape: `new T(...)` null-tested per arm (the shared `push 118h` hoisted); the creature re-read through an inline
// accessor for every call; UsePhysics a bool parameter loaded from memory (`xor ecx,ecx ; mov cl`, chunk-163 lesson). Local model.
void* __cdecl operator_new(unsigned int size);     // 0x00BFEA1A
inline void* operator new(unsigned int, void* where) { return where; }

struct CCharString { void* m_pData; };
struct CThingCreatureBase;

struct CCreatureActionBase {
    virtual void Release(long count);               // slot 0
    unsigned char _pad[0x116 - 4];
    bool m_fromScript;                              // +0x116
    unsigned char _pad2[0x118 - 0x117];
};

struct CCreatureAction_PlayCombatAnimation : CCreatureActionBase {
    CCreatureAction_PlayCombatAnimation(CThingCreatureBase* creature, const CCharString& animation, bool usePhysics);   // 0x008B7FF0
};
struct CCreatureAction_PlayCombatAnimationLooking : CCreatureActionBase {
    CCreatureAction_PlayCombatAnimationLooking(CThingCreatureBase* creature, const CCharString& animation, bool usePhysics);   // 0x008B80E0
};

struct CThingCreatureBase {
    bool IsPerformingAnyAction();                   // 0x006622A0
    void AddQueuedAction(CCreatureActionBase* action);   // 0x00664490
    bool SetCurrentAction(CCreatureActionBase* action);  // 0x006644F0
};

struct CTCScriptedControl {
    void* __vftable;
    CThingCreatureBase* PThing;                     // +0x04 the controlled creature
};

struct CAction {
    virtual void v00();
    CTCScriptedControl* PTCScript;                  // +0x04
    bool Activated;                                 // +0x08
    bool Terminated;                                // +0x09
    CThingCreatureBase* GetCreature() const { return PTCScript->PThing; }
};

struct CActionPlayCombatAnimation : CAction {
    bool        PlayedAnim;                         // +0x0c
    bool        UsePhysics;                         // +0x0d
    CCharString Animation;                          // +0x10
    bool        AddAsQueuedAction;                  // +0x14
    bool        WaitForAnimationToFinish;           // +0x15
    bool        AnimMayNeedCameraPositionUpdatedInSpeakAndWait;   // +0x16
    bool        AllowLooking;                       // +0x17
    void Update();
};

void CActionPlayCombatAnimation::Update()
{
    if (!PlayedAnim) {
        CCreatureActionBase* action;
        if (AllowLooking) {
            void* memory = operator_new(0x118);
            action = memory ? new(memory) CCreatureAction_PlayCombatAnimationLooking(GetCreature(), Animation, UsePhysics) : 0;
        } else {
            void* memory = operator_new(0x118);
            action = memory ? new(memory) CCreatureAction_PlayCombatAnimation(GetCreature(), Animation, UsePhysics) : 0;
        }
        action->m_fromScript = true;
        if (AddAsQueuedAction) {
            if (GetCreature()->IsPerformingAnyAction()) {
                GetCreature()->AddQueuedAction(action);
                PlayedAnim = true;
            } else {
                PlayedAnim = GetCreature()->SetCurrentAction(action);
            }
        } else {
            PlayedAnim = GetCreature()->SetCurrentAction(action);
        }
        if (!WaitForAnimationToFinish)
            Terminated = true;
        action->Release(1);
    } else if (!GetCreature()->IsPerformingAnyAction()) {
        Terminated = true;
    }
}
