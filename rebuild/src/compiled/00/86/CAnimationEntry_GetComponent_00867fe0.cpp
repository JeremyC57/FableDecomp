// CAnimationEntry::GetComponent(const CAnimComponentFollowingOnAnim** ppComponent) const -- the entry's own following-on-anim component (EAnimComponent 8) from its component map, else the shared default map's one, written through the out pointer
// Leaf: ??$GetComponent@VCAnimComponentFollowingOnAnim@@
// Grade: RELOCATION_MATCH (agent902) -- a port of the landed F88 twin 0x005E0380 (GetComponent<CAnimComponentFlags>, masked
//   Hamming distance 3: the type immediate): same-TU noinline LowerBound stand-in + one final *pp store with the default lookup
//   as if/else assignments. Ego_r.pdb ??$GetComponent@VCAnimComponentFollowingOnAnim@@@CAnimationEntry@@QBEXPAPBVCAnimComponentFollowingOnAnim@@@Z.
// Retail 0x00867FE0, 0x7D bytes, thiscall `ret 4`. Manifest label "CTCBase_GetInterfaceAbility" is a descriptive auto label.
//   The byte-identical twin at 0x00867F60 is the type 0x0B (CAnimComponentFollowingOnAnimFilter) instantiation.
// Callee: CVectorMap<EAnimComponent, CIVCountedPointer<CAnimComponentBase> >::LowerBound @ 0x005DFA50 (twice); default
//   component map @ 0x013B8C30 (Map +8). Caller: CCombatAction_GenericStrikeResponseKnockdownRecoverBase::Initialise 0x00865EB0.
enum EAnimComponent { ANIM_COMPONENT_FOLLOWING_ON_ANIM = 8 };

struct CAnimComponentBase {                                // CIVCountedPointeeBase-derived: vptr, reference count at +4
    virtual void v00();
    virtual void Delete();                                 // slot 1
    long IVRefCount;                                       // +0x04
};

class CAnimComponentFollowingOnAnim : public CAnimComponentBase {
public:
    static EAnimComponent StaticType() { return ANIM_COMPONENT_FOLLOWING_ON_ANIM; }   // GetType 0x005DF7D0 returns 8
};

template <class T> struct CIVCountedPointer {
    T* Object;
};

template <class K, class V> struct pair {
    K first;
    V second;
};

template <class K, class V> struct CVectorMap {            // retail: a 12-byte STLport vector + comparator/dirty bytes
    typedef pair<K, V> value_type;
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

struct CAnimationEntry {                                   // Ego_r.pdb, 0x10 bytes
    long                                 BankIndex;        // +0x00
    CDefString                           AnimName;         // +0x04
    CDefString                           GroupName;        // +0x08
    CIVCountedPointer<CAnimComponentMap> Components;       // +0x0c
    template <class T> void GetComponent(const T** ppComponent) const;
};

template <class T> void CAnimationEntry::GetComponent(const T** ppComponent) const
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
    *ppComponent = static_cast<const T*>(it->second.Object);
}

template void CAnimationEntry::GetComponent<CAnimComponentFollowingOnAnim>(const CAnimComponentFollowingOnAnim** ppComponent) const;