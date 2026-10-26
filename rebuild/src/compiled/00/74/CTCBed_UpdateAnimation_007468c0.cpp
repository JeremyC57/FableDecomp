// CTCBed::UpdateAnimation() -- while the bed animates: once the get-in animation has (almost) finished switch to the looping sleep animation, once the get-out animation has finished mark the bed empty; stop animating when the player has run out of animations
// Leaf: UpdateAnimation
// Retail: Fable.exe (TLC, VC7.1) @ 0x007468C0, 245 bytes (+11 int3 padding), `ret`.
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020 (inlined lookup of CTCAnimationComplex, type 0x5A),
//   CTCAnimationComplex::GetAnimation 0x0070B460, CAnimInit::Set 0x0070C050, CAnimateThing::SetMainAnim(const CAnimInit&,
//   long, long) 0x0070D580. Float constants 1.0f 0x0122DED8 (literal) and the 1e-4f epsilon variable 0x0129BA3C.
// Layout = Ego_r.pdb CTCBed (as the landed StartGetInBedAnimation 0x007469C0): Empty +0x0C, GettingIn +0x0D, GettingOut
//   +0x0E, Animating +0x0F, BedSleepAnim +0x10, PDef +0x14; the animation player's progress +0x1C and finished state
//   (+0x04 == +0x08, or bit 1 of +0x51).
// Shape: the landed sibling's model: the animation player comes from the inline accessor returning an unset local
//   (the absent path reads the never-written word); the progress is a named float read before the GettingIn test (and
//   popped on the path that never compares it); `progress > 1.0f - g_epsilon` (`test ah,41h ; jne`); the sleep loop
//   is a CAnimInit local built by its inline constructor over Set.
extern float g_epsilon;                         // 0x0129BA3C (1e-4f)
struct CTCBaseInterface;
struct CInterfaceMap {                          // a thing's sorted {type, component} vector map at +0x44
    struct SEntry { long type; CTCBaseInterface* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 UsingComponents
    CInterfaceMap m_interfaces;                 // +0x44 Components
    __forceinline bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
};
struct CBlendPair {                             // the two floats at CAnimInit +0x18, zeroed by their ctor
    float m_first;
    float m_second;
    CBlendPair() : m_first(0.0f), m_second(0.0f) { }
};
class CAnimInit { public:                              // 0x2c bytes
    long          Mode;                         // +0x00
    unsigned char AnimType;                     // +0x04
    bool          Flag05;                       // +0x05
    long          AnimIndex;                    // +0x08
    float         Scales[3];                    // +0x0c
    CBlendPair    Blend;                        // +0x18
    unsigned long Flags;                        // +0x20
    long          Arg24;                        // +0x24
    long          Percent;                      // +0x28
    void Set(long animType, long arg24, long animIndex, long mode, bool flag05, unsigned long flags, long percent);   // 0x0070C050
    CAnimInit(long animType, long arg24, long animIndex, long mode, bool flag05, unsigned long flags, long percent)
    {
        Set(animType, arg24, animIndex, mode, flag05, flags, percent);
    }
};
class CAnimateThing { public:                          // the thing's animation player
    unsigned char _pad00[0x04];
    void*         m_pCurrent;                   // +0x04
    void*         m_pEnd;                       // +0x08
    unsigned char _pad0c[0x1c - 0x0c];
    float         Progress;                     // +0x1c (0..1 through the main animation)
    unsigned char _pad20[0x51 - 0x20];
    unsigned char m_stateFlags;                 // +0x51 (bit 1: finished)
    void SetMainAnim(const CAnimInit& init, long mode, long delay);   // 0x0070D580
    bool IsFinished() const { return m_pCurrent == m_pEnd || (m_stateFlags & 2); }
};
class CTCAnimationComplex { public:
    CAnimateThing& GetAnimation();              // 0x0070B460
};
class CBedDef { public:
    unsigned char _pad00[0x28];
    long GetInBedAnimName;                      // +0x28 CDefString
    long SleepInBedAnimName;                    // +0x2c
    long GetUpFromBedAnimName;                  // +0x30
    long GetUpFromBedFastAnimName;              // +0x34
    bool Animating;                             // +0x38
};
class CTCBed {
public:
    virtual void v00();
    CThing* ParentThing;                        // +0x04
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
    unsigned char _pad0a[2];
    bool    Empty;                              // +0x0c
    bool    GettingIn;                          // +0x0d
    bool    GettingOut;                         // +0x0e
    bool    Animating;                          // +0x0f
    long    BedSleepAnim;                       // +0x10
    const CBedDef* PDef;                        // +0x14 CDefPointer<CBedDef const> object word
    __forceinline CTCAnimationComplex* GetAnimationComplex() const
    {
        CTCAnimationComplex* complex;
        if (ParentThing->HasTC(0x5a))
            complex = ParentThing->GetTC<CTCAnimationComplex>(0x5a);
        return complex;
    }
    void UpdateAnimation();
};


void CTCBed::UpdateAnimation()
{
    CAnimateThing& animate = GetAnimationComplex()->GetAnimation();
    if (Animating) {
        float progress = animate.Progress;
        if (GettingIn) {
            if (progress > 1.0f - g_epsilon) {
                GettingIn = false;
                CAnimInit init(6, 0, BedSleepAnim, 1, true, 0, 100);
                animate.SetMainAnim(init, 1, 0);
            }
        } else if (GettingOut) {
            if (progress > 1.0f - g_epsilon) {
                GettingOut = false;
                Empty = true;
            }
        }
        if (animate.IsFinished())
            Animating = false;
    }
}