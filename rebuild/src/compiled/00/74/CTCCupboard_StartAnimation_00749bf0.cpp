// CTCCupboard::StartAnimation(long anim) -- start the cupboard's open/close animation: mark it animating and play `anim` as the main anim (update mode 1, priority 100)
// Grade: RELOCATION_MATCH (retry 10, RTM 3077) -- HasTC + the ternary GetTC model assigned into an UNINITIALISED local (the miss arm reads the slot, `mov ecx,[esp+34h]`): both arms load the component straight into ecx
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
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                          // a thing's sorted {type, component} vector map at +0x44
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);       // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset<274> UsingComponents
    CInterfaceMap m_interfaces;                 // +0x44 Components
    template<class T> __forceinline T* GetTC(long wanted)        // the landed 0x00584157 lookup: the entry pick as one ternary, the component read once
    {
        ETCInterfaceType type = (ETCInterfaceType)(wanted);
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(type);
        return (T*)((CTCBaseInterface*)((it == map->last || wanted < it->first) ? map->last : it)->second);
    }
    unsigned long HasTC(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
};
class CTCBase { public:
    virtual void v00();
    CThing* ParentThing;                        // +0x04
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
};
class CAnimFrameInfo { public:
    float Current;
    float Last;
    CAnimFrameInfo() : Current(0), Last(0) {}
};
class CAnimInit { public:                              // 0x2c bytes
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
class CAnimateThing { public:
    void SetMainAnim(const CAnimInit& init, long mode, long delay);   // 0x0070D580
};
class CTCAnimationComplex { public:
    CAnimateThing& GetAnimation();              // 0x0070B460
};
class CTCCupboard : public CTCBase { public:
    bool Animating;                             // +0x0c
    void StartAnimation(long anim);
};

void CTCCupboard::StartAnimation(long anim)
{
    Animating = true;
    CAnimInit init;
    init.Set(6, 0, anim, 1, false, 0, 100);
    CTCAnimationComplex* animation;
    if (ParentThing->HasTC(0x5a))
        animation = ParentThing->GetTC<CTCAnimationComplex>(0x5a);
    CAnimateThing& animator = animation->GetAnimation();
    animator.SetMainAnim(init, 1, 0);
}