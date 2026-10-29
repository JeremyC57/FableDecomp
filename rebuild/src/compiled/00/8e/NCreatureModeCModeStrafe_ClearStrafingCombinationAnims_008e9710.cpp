// NCreatureMode::CModeStrafe::ClearStrafingCombinationAnims(bool) -- on a live parent with an animation component, finish the combination anims of types 0x1b and 0x19 (Finished set, ChangeAlphaBy = CurrentAlpha)
// Leaf: ClearStrafingCombinationAnims
// Retail: Fable.exe (TLC, VC7.1) @ 0x008E9710, 149 bytes (`ret 4`; the bool argument is unused).
// Callees: CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface, type 0x5a = CTCAnimationComplex: word 2 bit 26),
// CTCAnimationComplex::GetAnimation @ 0x0070B460, the CAnimateThing combination-anim lookup by type @ 0x0070C4E0.
// Layout: CAnimSet (retail, 12-byte vectors): CurrentAlpha +0x2c, ChangeAlphaBy +0x38, Finished = bit 1 of +0x51;
// CThing dead bit +0x91 bit 0; CModeBase::ParentThing +4.
// Shape: `if (parent && !dead && GetInterface(..))` (the template's bool is the third test); each found set gets an
// inline `FadeOut()` (test the bit, set it, copy the alpha).
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);   // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];         // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;             // +0x44
    unsigned char _pad4c[0x45];
    unsigned char m_dead : 1;               // +0x91 bit 0
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};

class CAnimSet { public:                           // retail layout (12-byte vectors)
    unsigned char _pad00[0x2c];
    float         CurrentAlpha;             // +0x2c
    float         LastAlpha;                // +0x30
    float         LastUpdateChange;         // +0x34
    float         ChangeAlphaBy;            // +0x38
    unsigned char _pad3c[0x15];
    unsigned char Looping : 1;              // +0x51 bit 0
    unsigned char Finished : 1;             // +0x51 bit 1
    void FadeOut()
    {
        if (!Finished) {
            Finished = 1;
            ChangeAlphaBy = CurrentAlpha;
        }
    }
};

class CAnimateThing { public:
    CAnimSet* FindCombinationAnim(long type);   // 0x0070C4E0
};
class CTCAnimationComplex { public:
    CAnimateThing& GetAnimation();          // 0x0070B460
};

namespace NCreatureMode {

struct CModeStrafe {
    virtual void v00();
    CThing* ParentThing;                    // +0x04
    void ClearStrafingCombinationAnims(bool immediately);
};

void CModeStrafe::ClearStrafingCombinationAnims(bool immediately)
{
    CTCAnimationComplex* animation;
    if (ParentThing && !ParentThing->m_dead && ParentThing->GetInterface(&animation, 0x5a)) {
        CAnimateThing& animator = animation->GetAnimation();
        CAnimSet* set = animator.FindCombinationAnim(0x1b);
        if (set)
            set->FadeOut();
        set = animator.FindCombinationAnim(0x19);
        if (set)
            set->FadeOut();
    }
}

}