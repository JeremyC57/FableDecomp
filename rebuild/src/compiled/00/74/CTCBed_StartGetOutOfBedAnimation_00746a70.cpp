// CTCBed::StartGetOutOfBedAnimation(long animation) -- for an animated bed: mark the bed as being got out of and play the get-up animation on the thing; the bed is no longer empty
// Leaf: StartGetOutOfBedAnimation
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00746A70, length 166 bytes (+ int3 padding), `ret 4`
// (?StartGetOutOfBedAnimation@CTCBed@@QAEXJ@Z). Called from 0x0092CFC0, 0x0092DFAA, 0x0092E06D.
// Label correction: the catalog name "StartGetInBedAnimation" is the sibling at 0x007469C0 (`ret 8`); this one-argument body
// clears GettingIn and sets GettingOut.
// Callees: CAnimInit::Set @ 0x0070C050, CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (inlined GetInterface
// for CTCAnimationComplex, type 0x5A), CTCAnimationComplex::GetAnimation @ 0x0070B460, CAnimateThing::SetMainAnim @ 0x0070D580.
// Layout = Ego_r.pdb CTCBed (24 bytes): Empty +0x0C, GettingIn +0x0D, GettingOut +0x0E, Animating +0x0F, PDef +0x14.
// Shape: as StartGetInBedAnimation (HasTC + one-ternary GetTC into an unset local); the key and the never-initialised out pointer share the dead `animation` slot.
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
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(type);
        return (T*)((CTCBaseInterface*)((it == map->last || interfaceType < it->first) ? map->last : it)->second);
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
class CAnimateThing { public:
    void SetMainAnim(const CAnimInit& init, long mode, long delay);   // 0x0070D580
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
    void StartGetInBedAnimation(long animation, long sleepAnimation);
    void StartGetOutOfBedAnimation(long animation);
};

void CTCBed::StartGetOutOfBedAnimation(long animation)
{
    if (PDef->Animating) {
        GettingIn = false;
        GettingOut = true;
        CAnimInit init(6, 0, animation, 1, false, 0, 100);
        CAnimateThing& animate = GetAnimationComplex()->GetAnimation();
        animate.SetMainAnim(init, 1, 0);
        Animating = true;
    }
    Empty = false;
}