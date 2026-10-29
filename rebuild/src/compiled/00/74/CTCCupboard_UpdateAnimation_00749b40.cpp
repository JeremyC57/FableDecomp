// CTCCupboard::UpdateAnimation() -- while the cupboard is animating, advance its animation by one frame (1 / fps seconds) and stop once the animation has (to within epsilon) reached its end
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00749B40, 163 bytes (plain `ret`).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (inlined lookup of CTCAnimationComplex, interface 0x5a:
// bit 26 of the +0x28 word), CTCAnimationComplex::GetAnimation @ 0x0070B460, CAnimateThing::UpdateFrameLogic(const
// CAnimFrameTime&) @ 0x0070E8B0 (as the landed twin CTCCreatureHive::UpdateAnimation 0x0077D190). Globals: g_framesPerSecond
// @ 0x01375550, 1.0f @ 0x0122DED8, g_epsilon_0129ba3c @ 0x0129BA3C.
// Layout: Ego_r.pdb CTCCupboard Animating +0x0c; the animation's play fraction at +0x1c (named descriptively).
// Shape: unlike the hive twin the animation is fetched (an accessor returning an unset local when the component is absent,
// over a one-ternary GetTC, so
// the component lands straight in ecx and the absent arm reads the key's frame word)
// and the 12-byte frame-time record is zero-constructed before the Animating test; the seconds are stored inside;
// `1.0f - eps < fraction` is `fld 1.0 ; fsub eps ; fcomp [esi+1Ch] ; test ah,5 ; jp`.
extern long g_framesPerSecond;                          // 0x01375550
extern float g_epsilon_0129ba3c;                        // 0x0129BA3C (1e-4f)
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                                  // CVectorMap<ETCInterfaceType, CTCBase*>
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);               // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                     // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                         // +0x44
    __forceinline unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(type);
        return (it == map->last || interfaceType < it->first ? map->last : it)->second;
    }
    __forceinline CTCBase* GetTCIfPresent(long interfaceType)
    {
        CTCBase* tc;                                    // deliberately unset when absent (retail reads a dead slot)
        if (HasTC(interfaceType))
            tc = GetTC(interfaceType);
        return tc;
    }
};
struct CAnimFrameTime {                                 // 12-byte UpdateFrameLogic argument
    float Seconds;
    long  Reserved1;
    long  Reserved2;
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
class CTCCupboard { public:
    virtual void v00();
    CThing* ParentThing;                                // +0x04 (CTCBase)
    unsigned char _pad08[0x0c - 0x08];
    bool Animating;                                     // +0x0c
    void UpdateAnimation();
};

void CTCCupboard::UpdateAnimation()
{
    CAnimateThing& anim = ((CTCAnimationComplex*)ParentThing->GetTCIfPresent(0x5a))->GetAnimation();
    CAnimFrameTime time;
    if (Animating) {
        time.Seconds = 1.0f / g_framesPerSecond;
        anim.UpdateFrameLogic(time);
        if (1.0f - g_epsilon_0129ba3c < anim.PlayFraction)
            Animating = false;
    }
}