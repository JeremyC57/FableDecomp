// CTCDoor::UpdateAnimation() -- once the door animation has (to within epsilon) finished, stop updating a proximity door that is not moving, else clear the opening/closing state; then advance the animation by one frame (slowed by bullet time when the door is affected by it)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007308F0, 263 bytes (+ int3 padding), plain `ret`.
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020 (inlined lookup of CTCAnimationComplex,
//   interface 0x5a: bit 26 of the +0x28 word), CTCAnimationComplex::GetAnimation @ 0x0070B460, CThing::
//   SetForceFrameUpdate(bool) @ 0x004C7C60, CTCBase::GetGameSubsystem @ 0x00686D60 (out of line, result unused),
//   CTCBase::GetWorld @ 0x00686A70 (x2), CThing::IsAffectedByBulletTime @ 0x004C9920, the bullet-time object's
//   GetTimeScale @ 0x004C5FA0, CAnimateThing::UpdateFrameLogic(const CAnimFrameTime&) @ 0x0070E8B0.
// Globals: g_framesPerSecond @ 0x01375550, 1.0f @ 0x0122DED8, g_epsilon_0129ba3c (1e-4f) @ 0x0129BA3C.
// Layout: engine/CTCDoor.h (retail offsets): Opening +0x12, Closing +0x13, PDef +0x20; CDoorDef (Ego_r.pdb)
//   TriggerType +0x28; CWorld +0x68 the bullet-time state (active flag +0x0c, a second non-zero test +0x04); the
//   animation's play fraction +0x1c and the 12-byte frame-time record as in the landed CTCCupboard::UpdateAnimation
//   0x00749B40. Local model (the header is a flat pack(1) struct).
// Shape: `T* tc = 0; if (HasTC) tc = GetTC;` for retail's `xor ecx,ecx` preload; `1.0f - eps < fraction` gives
//   `fld 1.0 ; fsub eps ; fcomp [edi+1Ch] ; test ah,5 ; jp`; the frame-time record is constructed after the state test;
//   the bullet-time test is an inline `||` on one GetWorld result, the scale read through a second GetWorld. The
//   record's two trailing words are modelled as floats: as longs (landed 0x00749B40) their zero stores make VC7.1 keep
//   a zero register (ebx) and fold every bool test / `push 0` / zero store into it; float zeros are immediates.
extern long g_framesPerSecond;                          // 0x01375550
extern float g_epsilon_0129ba3c;                        // 0x0129BA3C (1e-4f)
class CTCBase;
struct CInterfaceMap {                                  // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                     // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template<class T> __forceinline T* GetTC(long wanted)
    {
        long type = wanted;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || wanted < it->type) ? map->last : it)->component;
    }
    void SetForceFrameUpdate(bool force);               // 0x004C7C60
    bool IsAffectedByBulletTime();                      // 0x004C9920
};
struct CAnimFrameTime {                                 // 12-byte UpdateFrameLogic argument
    float Seconds;
    float Reserved1;
    float Reserved2;
    CAnimFrameTime() : Seconds(0), Reserved1(0), Reserved2(0) { }
};
class CAnimateThing { public:
    unsigned char _pad00[0x1c];
    float PlayFraction;                                 // +0x1c
    void UpdateFrameLogic(const CAnimFrameTime& time);  // 0x0070E8B0
};
class CTCAnimationComplex { public:
    CAnimateThing& GetAnimation();                      // 0x0070B460
};
struct CBulletTime {
    unsigned char _pad00[4];
    long m_slowdown;                                    // +0x04
    unsigned char _pad08[4];
    bool m_active;                                      // +0x0c
    bool IsActive() const { return m_active || m_slowdown != 0; }
    float GetTimeScale();                               // 0x004C5FA0
};
class CWorld { public:
    unsigned char _pad00[0x68];
    CBulletTime* PBulletTime;                           // +0x68
};
struct CGameSubsystem;
class CDoorDef { public:
    unsigned char _pad00[0x28];
    long TriggerType;                                   // +0x28 EDoorTriggerType
};
class CTCDoor { public:
    virtual void v00();
    CThing* ParentThing;                                // +0x04 (CTCBase)
    unsigned char _pad08[0x10 - 0x08];
    bool CurrentOpen;                                   // +0x10
    bool OldOpen;                                       // +0x11
    bool Opening;                                       // +0x12
    bool Closing;                                       // +0x13
    long LastCreatureFoundWF;                           // +0x14
    long UpdateID;                                      // +0x18
    long TriggerType;                                   // +0x1c
    const CDoorDef* PDef;                               // +0x20 CDefPointer<CDoorDef const>::Object
    CGameSubsystem* GetGameSubsystem();                 // 0x00686D60 (CTCBase)
    CWorld* GetWorld();                                 // 0x00686A70 (CTCBase)
    void UpdateAnimation();
};

void CTCDoor::UpdateAnimation()
{
    CTCAnimationComplex* tc = 0;
    if (ParentThing->HasTC(0x5a))
        tc = ParentThing->GetTC<CTCAnimationComplex>(0x5a);
    CAnimateThing& anim = tc->GetAnimation();
    if (1.0f - g_epsilon_0129ba3c < anim.PlayFraction) {
        if (PDef->TriggerType == 1 && !Opening && !Closing) {
            ParentThing->SetForceFrameUpdate(false);
            return;
        }
        Opening = false;
        Closing = false;
    }
    CAnimFrameTime time;
    GetGameSubsystem();
    time.Seconds = 1.0f / g_framesPerSecond;
    if (GetWorld()->PBulletTime->IsActive() && ParentThing->IsAffectedByBulletTime())
        time.Seconds /= GetWorld()->PBulletTime->GetTimeScale();
    anim.UpdateFrameLogic(time);
}