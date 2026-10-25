// CAnimationEntry::GetComponent(CAnimComponentStrikeSpeedMultiplier** ppComponent) -- the entry's StrikeSpeedMultiplier component (EAnimComponent 9) from its own component map, else the shared default map's one, written through the out pointer
// Leaf: ??$GetComponent@VCAnimComponentStrikeSpeedMultiplier@@@CAnimationEntry@@QAEXPAPAVCAnimComponentStrikeSpeedMultiplier@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0085AD30, 128 bytes (+ int3 padding), thiscall, `ret 4`.
// Identity: GetComponent<CAnimComponentStrikeSpeedMultiplier> (non-const, QAE) -- the type immediate 9 is CAnimComponentStrikeSpeedMultiplier's GetType
//   (vtable 0x01255800 slot 2 returns 9); Ego_r / FableWin.pdb name set `??$GetComponent@VCAnimComponentStrikeSpeedMultiplier@@@CAnimationEntry@@QAEXPAPAVCAnimComponentStrikeSpeedMultiplier@@@Z`. Caller: the landed CCombatActionBase::GetFrameUpdateTime 0x008571C0 (strike speed multiplier); the name sets hold only the non-const (QAE) instantiation.
//   Family G012 (10 bodies differing only in the type immediate; each its own COMDAT: retail has no ICF); the manifest
//   label "CThingCreatureBase_GetCombatActionFrameTime" is propagated/behavioural. Template twin of the landed GetComponent<CAnimComponentFlags> 0x005E0380.
// Callees: CVectorMap<EAnimComponent, CIVCountedPointer<CAnimComponentBase> >::LowerBound 0x005DFA50 (twice).
// Globals: the default component map 0x013B8C30 (its Map vector at +8 = 0x013B8C38, end at 0x013B8C3C).
// Shape: `if (!map || (it = map->Map.find(type)) == map->Map.end()) { found = Default.LowerBound(type); it = found valid ? found
//   : Default.end(); } *pp = it->second;` -- LowerBound needs its body in the TU (noinline) so VC sees the key unchanged.
enum EAnimComponent { ANIM_COMPONENT_FLAGS = 0, ANIM_COMPONENT_STRIKE_SPEED_MULTIPLIER = 9 };

struct CAnimComponentBase {                                // CIVCountedPointeeBase-derived: vptr, reference count at +4
    virtual void v00();
    virtual void Delete();                                 // slot 1
    long IVRefCount;                                       // +0x04
};

class CAnimComponentStrikeSpeedMultiplier : public CAnimComponentBase {
public:
    long Data;                                             // +0x08
    static EAnimComponent StaticType() { return ANIM_COMPONENT_STRIKE_SPEED_MULTIPLIER; }
};

template <class T> struct CIVCountedPointer {
    T* Object;
};

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
}

template <class K, class V> struct CVectorMap {            // retail: a 12-byte STLport vector + comparator/dirty bytes
    typedef std::pair<K, V> value_type;
    value_type* m_first;                                   // +0x00
    value_type* m_last;                                    // +0x04
    value_type* m_end;                                     // +0x08
    unsigned char Compare;                                 // +0x0c
    bool Dirty;                                            // +0x0d
    const value_type* end() const { return m_last; }
    __declspec(noinline) const value_type* LowerBound(const K& key) const   // 0x005DFA50: same-TU read-only stand-in
    {
        const value_type* lo = m_first;
        int len = m_last - m_first;
        while (len > 0) {
            int half = len >> 1;
            const value_type* middle = lo + half;
            if (middle->first < key) {
                lo = middle + 1;
                len = len - half - 1;
            }
            else
                len = half;
        }
        return lo;
    }
    const value_type* find(const K& key) const
    {
        const value_type* it = LowerBound(key);
        return (it == end() || key < it->first) ? end() : it;
    }
};

struct CAnimComponentMap : CAnimComponentBase {            // vptr, count, Map at +8
    CVectorMap<EAnimComponent, CIVCountedPointer<CAnimComponentBase> > Map;   // +0x08
};

extern CAnimComponentMap g_DefaultAnimComponents;          // 0x013B8C30

struct CDefString {
    long TablePos;
};

class CAnimationEntry {                                    // Ego_r.pdb, 0x10 bytes
public:
    long                                 BankIndex;        // +0x00
    CDefString                           AnimName;         // +0x04
    CDefString                           GroupName;        // +0x08
    CIVCountedPointer<CAnimComponentMap> Components;       // +0x0c
    template <class T> void GetComponent(T** ppComponent);
};

template <class T> void CAnimationEntry::GetComponent(T** ppComponent)
{
    typedef CVectorMap<EAnimComponent, CIVCountedPointer<CAnimComponentBase> >::value_type value_type;
    const EAnimComponent type = T::StaticType();
    const CAnimComponentMap* map = Components.Object;
    const value_type* it;
    if (map == 0 || (it = map->Map.find(type)) == map->Map.end())
    {
        const value_type* found = g_DefaultAnimComponents.Map.LowerBound(type);
        if (found != g_DefaultAnimComponents.Map.end() && !(type < found->first))
            it = found;
        else
            it = g_DefaultAnimComponents.Map.end();
    }
    *ppComponent = static_cast<T*>(it->second.Object);
}

template void CAnimationEntry::GetComponent<CAnimComponentStrikeSpeedMultiplier >(CAnimComponentStrikeSpeedMultiplier** ppComponent);