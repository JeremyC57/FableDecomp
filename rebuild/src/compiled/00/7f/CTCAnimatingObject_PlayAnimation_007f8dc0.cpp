// CTCAnimatingObject::PlayAnimation(const CAnimInit& init, long priority) -- when the thing has an animation component: its animator plays `init` as the main anim (mode 1, no delay), then Animating is set and the priority stored
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007F8DC0, length 95 bytes.
// Leaf: PlayAnimation
// Callees: CVectorMap::LowerBound @ 0x0040F020, CTCAnimationComplex::GetAnimation @ 0x0070B460 (`return *PAnimation`; the
// manifest's "CWorld::DrawGetEnvironment" is propagated), CAnimateThing::SetMainAnim(const CAnimInit&, long, long) @ 0x0070D580. Interface 0x5a (CTCAnimationComplex) = word 2, bit 26 of
// the presence bitset (`test dword ptr [eax+28h],4000000h`). Ego_r.pdb CAnimSet: UpdateMode +0, CurrentPercentageComplete
// +0x24 / LastPercentageComplete +0x28 (retail +0x1c / +0x20 behind two 12-byte vectors); CTCAnimatingObject: Animating
// +0x0c, CurrentAnimationPriority +0x10.
// Ego_r.pdb: `?PlayAnimation@CTCAnimatingObject@@AAEXABVCAnimInit@@J@Z` (`ret 8`).
// Shape: here the GetInterface result is tested (the absent path returns at once). The animator is fetched BEFORE the
// SetMainAnim arguments are pushed, so it is a named local; written inline in the call, VC7.1 pushes first.
struct CTCBaseInterface;
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                         // a thing's sorted {type, component} vector map at +0x44
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);      // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];            // +0x20 std::bitset<274> UsingComponents
    CInterfaceMap m_interfaces;                // +0x44 Components
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)((CTCBaseInterface*)it->second);
            return true;
        }
        return false;
    }
};

class CTCBase { public:                               // engine/CTCBase.h layout, modelled polymorphic
    virtual void v00();
    CThing* ParentThing;                       // +0x04
    bool    ToBeRemoved;                       // +0x08
    bool    IsInGlobalUpdateTCs;               // +0x09
};

class CAnimInit;

class CAnimSet { public:                              // CAnimateThing::CurrentAnim.MainAnimSet (retail layout)
    long  UpdateMode;                          // +0x00 EAnimUpdateMode
    unsigned char _vectors[0x18];              // +0x04 Anims, InternalAlphas (12-byte retail vectors)
    float CurrentPercentageComplete;           // +0x1c
    float LastPercentageComplete;              // +0x20
};

class CAnimateThing { public:
    CAnimSet CurrentAnim;                      // +0x00 (the CCompleteAnimation's MainAnimSet)
    void SetMainAnim(const CAnimInit& init, long mode, long delay);   // 0x0070D580
};

class CTCAnimationComplex { public:
    CAnimateThing& GetAnimation();             // 0x0070B460 (`mov eax,[ecx+0Ch]`, PAnimation)
};

class CTCAnimatingObject : public CTCBase { public:
    bool Animating;                            // +0x0c
    long CurrentAnimationPriority;             // +0x10
    void SetCurrentAnimAsManualUpdate();
    void SetFractionComplete(float fraction);
    void PlayAnimation(const CAnimInit& init, long priority);
};

void CTCAnimatingObject::PlayAnimation(const CAnimInit& init, long priority)
{
    CTCAnimationComplex* animation;
    if (ParentThing->GetInterface(&animation, 0x5a)) {
        CAnimateThing& animator = animation->GetAnimation();
        animator.SetMainAnim(init, 1, 0);
        Animating = true;
        CurrentAnimationPriority = priority;
    }
}