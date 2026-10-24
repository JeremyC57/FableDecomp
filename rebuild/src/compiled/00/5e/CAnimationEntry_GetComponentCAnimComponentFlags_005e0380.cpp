// CAnimationEntry::GetComponent<CAnimComponentFlags>(const CAnimComponentFlags** ppComponent) -- the entry's flags component (type 0) from its own component map, else the shared default map's one, written through the out pointer
// Grade: RELOCATION_MATCH (near-miss retry agent901) -- same-TU noinline LowerBound stand-in (key folded to the immediate, one key store before the null test) + one final *pp store over the entry-map result, with the default lookup written as if/else assignments so VC tail-duplicates the store into each default arm (family F88)
// Leaf: ??$GetComponent@UCAnimComponentFlags@@@CAnimationEntry@@QBEXPAPBUCAnimComponentFlags@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005E0380, 125 bytes, `ret 4` (__fastcall).
//   Ego_r.pdb: `??$GetComponent@VCAnimComponentFlags@@@CAnimationEntry@@QBEXPAPBVCAnimComponentFlags@@@Z`.
// Callees: 0x005DFA50 CVectorMap<EAnimComponent, CIVCountedPointer<CAnimComponentBase> >::LowerBound (twice).
// Globals: the default component map @ 0x013B8C30 (its Map vector at +8 = 0x013B8C38, end at 0x013B8C3C).
// Callers: CAnimationEntry::GetAnimateThingFlags 0x005DC390 (landed), 0x005DC215, 0x0073A9D7. Template twins:
//   0x005E0280 (TransitionInTime, type 1), 0x005E0300.
// Shape: `if (!map || (it = map->Map.find(type)) == map->Map.end()) { found = Default.LowerBound(type); it = found valid ? found
//   : Default.end(); } *pp = it->second;` -- one store tail that VC tail-duplicates into the two default arms (the original
//   stays last for the entry-map hit); LowerBound (0x005DFA50) needs its body in the TU (noinline) so VC sees the key
//   unchanged: one key word zeroed before the null test, both compares folded to the immediate 0.
// Label correction: catalogued as `AnimationDef_GetAnimationFlags`; the PDB name is the GetComponent template.
enum EAnimComponent { ANIM_COMPONENT_FLAGS = 0, ANIM_COMPONENT_TRANSITION_IN_TIME = 1 };

struct CAnimComponentBase {                                // CIVCountedPointeeBase-derived: vptr, reference count at +4
    virtual void v00();
    virtual void Delete();                                 // slot 1
    long IVRefCount;                                       // +0x04
};

struct CAnimComponentFlags : CAnimComponentBase {
    long Flags;                                            // +0x08
    static EAnimComponent StaticType() { return ANIM_COMPONENT_FLAGS; }
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

template void CAnimationEntry::GetComponent<CAnimComponentFlags>(const CAnimComponentFlags** ppComponent) const;