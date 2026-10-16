// CTCCupboard::StartAnimation(long anim) -- start the cupboard's open/close animation: mark it animating and play `anim` as the main anim (update mode 1, priority 100)
// Grade: DIFFER(140v138) -- only the component register: retail loads the CTCAnimationComplex straight into ecx in both GetInterface arms (`mov ecx,[eax+4] ; jmp ; mov ecx,[esp+34h]`), ours into eax plus `mov ecx,eax` (same as the landed CTCAnimatingObject/CTCCreatureHive rows, whose retail does use eax); tried out-param, unset-local accessor, optimize(s), declaration order
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00749BF0, length 138 bytes (`ret 4`).
// Callees: CAnimInit::Set (seven arguments) @ 0x0070C050 (landed as "CInitRumble::Set"; its stores are the CAnimInit fields:
// Type +4, TypeFlags +0x24, Anim +8, UpdateMode +0, Looping +5, Flags +0x20, Priority +0x28 and the 1.0 speeds/alphas),
// CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020 (inlined GetInterface for type 0x5A = CTCAnimationComplex,
// result unchecked), CTCAnimationComplex::GetAnimation @ 0x0070B460, CAnimateThing::SetMainAnim(const CAnimInit&, long, long)
// @ 0x0070D580.
// Layout: Ego_r.pdb CTCCupboard: Animating +0xC. CAnimInit (0x2C) with CAnimFrameInfo Frame at +0x18 (inline zeroing ctor).
// Shape: the animator is a named local (fetched before the SetMainAnim pushes); the frame-info ctor's zero stores land
// between the Set pushes.
struct CTCBaseInterface;
struct CInterfaceMap {                          // a thing's sorted {type, component} vector map at +0x44
    struct SEntry { long type; CTCBaseInterface* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset<274> UsingComponents
    CInterfaceMap m_interfaces;                 // +0x44 Components
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
struct CTCBase {
    virtual void v00();
    CThing* ParentThing;                        // +0x04
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
};
struct CAnimFrameInfo {
    float Current;
    float Last;
    CAnimFrameInfo() : Current(0), Last(0) {}
};
struct CAnimInit {                              // 0x2c bytes
    long  UpdateMode;                           // +0x00
    unsigned char Type;                         // +0x04
    bool  Looping;                              // +0x05
    long  Anim;                                 // +0x08
    float Speed;                                // +0x0c
    float Alpha;                                // +0x10
    float LastAlpha;                            // +0x14
    CAnimFrameInfo Frame;                       // +0x18
    unsigned long Flags;                        // +0x20
    unsigned long TypeFlags;                    // +0x24
    long  Priority;                             // +0x28
    CAnimInit() {}
    void Set(unsigned char type, unsigned long typeFlags, long anim, long updateMode, bool looping, unsigned long flags,
             long priority);                    // 0x0070C050
};
struct CAnimateThing {
    void SetMainAnim(const CAnimInit& init, long mode, long delay);   // 0x0070D580
};
struct CTCAnimationComplex {
    CAnimateThing& GetAnimation();              // 0x0070B460
};
struct CTCCupboard : CTCBase {
    bool Animating;                             // +0x0c
    void StartAnimation(long anim);
};

void CTCCupboard::StartAnimation(long anim)
{
    Animating = true;
    CAnimInit init;
    init.Set(6, 0, anim, 1, false, 0, 100);
    CTCAnimationComplex* animation;
    ParentThing->GetInterface(&animation, 0x5a);
    CAnimateThing& animator = animation->GetAnimation();
    animator.SetMainAnim(init, 1, 0);
}
