// Grade: DIFFER(120v125) -- retail zeroes ONE key word before the null test (shared by both LowerBound calls, as here) but compares both results against the immediate 0 and keeps three separate *pp tails; the const local key here is re-read for each compare (mov reg,[esp+8] ; cmp reg,[eax] ; jge) and the found tail is cross-jumped (known family gap F88, FINDINGS_LOG: 10 two-level CVectorMap lookups, best 120v125). A by-value find(K) over LowerBound(const long&) folds the compares (cmp [eax],0) but zeroes the key word inside each branch (107v125).
// CAnimationEntry::GetComponent<CAnimComponentFlags>(const CAnimComponentFlags** ppComponent) -- the entry's flags component (type 0) from its own component map, else the shared default map's one, written through the out pointer
// Leaf: GetComponent
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005E0380, 125 bytes, `ret 4` (__thiscall).
//   Ego_r.pdb: `??$GetComponent@VCAnimComponentFlags@@@CAnimationEntry@@QBEXPAPBVCAnimComponentFlags@@@Z`.
// Callees: 0x005DFA50 CVectorMap<EAnimComponent, CIVCountedPointer<CAnimComponentBase> >::LowerBound (twice).
// Globals: the default component map @ 0x013B8C30 (its Map vector at +8 = 0x013B8C38, end at 0x013B8C3C).
// Callers: CAnimationEntry::GetAnimateThingFlags 0x005DC390 (landed), 0x005DC215, 0x0073A9D7. Template twins:
//   0x005E0280 (TransitionInTime, type 1), 0x005E0300.
// Shape: `if (Components) { it = Components->Map.find(type); if (it != Components->Map.end()) { *pp = it->second; return; } }
//   *pp = DefaultMap.Map.find(type)->second;` -- CVectorMap::find is the inline ternary `(it == end() || key < it->first)
//   ? end() : it` over the out-of-line LowerBound; retail keeps one key word for both lookups
//   (zeroed before the null test, the compares folded to the immediate 0; here a const local key), the
//   outer end() test re-reads the map's last pointer, and the default lookup is unchecked (end()->second when absent).
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
    const value_type* LowerBound(const K& key) const;     // 0x005DFA50
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
    const EAnimComponent type = T::StaticType();
    if (Components.Object) {
        const CAnimComponentMap* map = Components.Object;
        const CVectorMap<EAnimComponent, CIVCountedPointer<CAnimComponentBase> >::value_type* it = map->Map.find(type);
        if (it != map->Map.end()) {
            *ppComponent = static_cast<const T*>(it->second.Object);
            return;
        }
    }
    *ppComponent = static_cast<const T*>(g_DefaultAnimComponents.Map.find(type)->second.Object);
}

template void CAnimationEntry::GetComponent<CAnimComponentFlags>(const CAnimComponentFlags** ppComponent) const;
