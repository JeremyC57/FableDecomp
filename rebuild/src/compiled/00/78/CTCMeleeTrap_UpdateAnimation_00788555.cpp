// CTCMeleeTrap::UpdateAnimation() -- step the trap's animation by the thing's update time; while a triggered, non-continuous trap's opener is open start the continuous loop (type 2) or reset the trap, and once it has closed again finish the trigger (a type-4 trap kills itself)
// Leaf: UpdateAnimation
// Retail: Fable.exe (TLC, VC7.1) @ 0x00788555, 194 bytes (plain `ret`; PDB ?UpdateAnimation@CTCMeleeTrap@@AAEXXZ). The trap TU is
//   size-optimised with a frame pointer: `optimize("y", off)` + `("s", on)` (`push 5Ah ; pop ebx`, `leave`).
// Callees: CTCBase::GetWorld() @ 0x00686A70, GetUpdateTimeForThing(CThing*, CWorld*) @ 0x00787C62 (`__fastcall`; the PDB's
//   file-static ?GetUpdateTimeForThing@?A0xedf0dd2a@@YIMABVCThing@@ABVCWorld@@@Z), std::bitset::test @ 0x00410DE0 (out of line in this
//   TU), CVectorMap<...>::LowerBound @ 0x0040F020 (interface 0x5a), the animation component's GetAnimation() @ 0x0070B460,
//   CFadeInFadeOutBase::IsOpen() @ 0x0048DA30, CTCMeleeTrap::InitialiseContinuousPlay() @ 0x00788198 (InitialiseAnimation(name,
//   true) + PlayingContinuous = true; manifest "CEngine::ChangeCameraAngle"), CTCTrapBase::Reset() @ 0x00787BF1 (Closing on,
//   Opening off, virtual ResetTrap; manifest "...::Cleanup"), CThing::Kill(bool) @ 0x004C9B80, CAnimateThing::UpdateFrameLogic(
//   const CAnimFrameTime&) @ 0x0070E8B0.
// Layout: Ego_r.pdb CTCTrapBase with 12-byte retail vectors (TrapOpener +0x38 = CFadeInFadeOutBase {Active +0, Closing +0xc,
//   Opening +0xd}, Triggered +0x59, TrapType +0x5c) and CTCMeleeTrap PlayingContinuous +0x60.
// Shape: the thing is read into a local before GetWorld(); the frame-time temporary zeroes its other two fields in the
//   initialiser list and takes the time in the body (the call result stays on the x87 stack until last); the animation
//   component is the unchecked lookup (absent: the never-written slot, as the shared [ebp-4] key/out word) feeding a reference.
#pragma optimize("y", off)
#pragma optimize("s", on)
class CTCBase;
class CWorld;
struct CAnimFrameTime {                                 // 12 bytes
    float Time;
    float Offset;
    float Blend;
    CAnimFrameTime(float time) : Offset(0.0f), Blend(0.0f) { Time = time; }
};
class CAnimateThing { public:
    void UpdateFrameLogic(const CAnimFrameTime& time);  // 0x0070E8B0
};
class CTCAnimationComplex { public:                                   // interface 0x5a
public:
    CAnimateThing& GetAnimation();   // 0x0070B460
};
struct CInterfaceMap {                                  // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
struct CComponentBitset {                               // std::bitset<274>
    unsigned long m_words[9];
    bool test(long bit);                                // 0x00410DE0 (std::bitset::test)
};
class CThing {
public:
    unsigned char    _pad00[0x20];
    CComponentBitset UsingComponents;                   // +0x20
    CInterfaceMap    m_interfaces;                      // +0x44
    template <class T> __forceinline T* PeekTC(long type)
    {
        T* tc;
        if (UsingComponents.test(type)) {
            long key = type;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&key);
            tc = (T*)((it == map->last || type < it->type) ? map->last : it)->component;
        }
        return tc;
    }
    void Kill(bool immediately);                        // 0x004C9B80
};
float __fastcall GetUpdateTimeForThing(CThing* thing, CWorld* world);   // 0x00787C62 (file-static in the PDB)
class CFadeInFadeOutBase { public:                             // 0x1c
    bool          Active;                               // +0x00
    float         FadeInTime;                           // +0x04
    float         FadeOutTime;                          // +0x08
    bool          Closing;                              // +0x0c
    bool          Opening;                              // +0x0d
    float         OpenTimer;                            // +0x10
    float         CloseTimer;                           // +0x14
    unsigned long ToColour;                             // +0x18 CRGBColour
    bool IsOpen();                                      // 0x0048DA30
};
class CTCBase
{
public:
    virtual void v00();
protected:
    CWorld& GetWorld() const;   // 0x00686A70
};

class CTCTrapBase : public CTCBase { public:
    CThing*       ParentThing;                          // +0x04 (CTCBase)
    unsigned char _pad08[0x38 - 0x08];
    CFadeInFadeOutBase TrapOpener;                      // +0x38 (PDB +0x44)
    const void*   PTrapDef;                             // +0x54
    bool          Active;                               // +0x58
    bool          Triggered;                            // +0x59
    bool          LastTriggerOnProximity;               // +0x5a
    long          TrapType;                             // +0x5c ETrapType
    void Reset();                                       // 0x00787BF1
};
struct CTCMeleeTrap : CTCTrapBase {
    bool PlayingContinuous;                             // +0x60
    bool StrikeAnimation;                               // +0x61
    void InitialiseContinuousPlay();                    // 0x00788198
    void UpdateAnimation();
};

void CTCMeleeTrap::UpdateAnimation()
{
    CThing* thing = ParentThing;
    CAnimFrameTime time(GetUpdateTimeForThing(thing, (&GetWorld())));
    CAnimateThing& animation = ParentThing->PeekTC<CTCAnimationComplex>(0x5a)->GetAnimation();
    if (!PlayingContinuous && Triggered) {
        if (TrapOpener.IsOpen()) {
            if (TrapType == 2)
                InitialiseContinuousPlay();
            else
                Reset();
        } else if (!TrapOpener.Active) {
            if (TrapType == 4)
                ParentThing->Kill(true);
            Triggered = false;
        }
    }
    animation.UpdateFrameLogic(time);
}