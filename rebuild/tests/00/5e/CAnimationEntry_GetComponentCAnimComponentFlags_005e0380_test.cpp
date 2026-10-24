// Behaviour test for 0x005E0380 CAnimationEntry::GetComponent<CAnimComponentFlags>(const T**) const: the entry's own
// type-0 component when its component map has one, else the default map's (its end()->second when absent).
#include <stdio.h>
#include <string.h>
#ifndef _MSC_VER
#define __forceinline inline
#define __declspec(x)
#endif
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

void CAnimComponentBase::v00() {}
void CAnimComponentBase::Delete() {}
CAnimComponentMap g_DefaultAnimComponents;

typedef CVectorMap<EAnimComponent, CIVCountedPointer<CAnimComponentBase> >::value_type TPair;

static void SetMap(CVectorMap<EAnimComponent, CIVCountedPointer<CAnimComponentBase> >& m, TPair* first, unsigned int n)
{
    m.m_first = first; m.m_last = first + n; m.m_end = first + n;
}

int main()
{
    CAnimComponentFlags ownFlags, defFlags, sentinel;
    ownFlags.Flags = 1; defFlags.Flags = 2; sentinel.Flags = 3;
    int ok = 1;

    // default map: {0 -> defFlags, 1 -> null}, then a sentinel slot past end
    TPair defPairs[3];
    defPairs[0].first = ANIM_COMPONENT_FLAGS; defPairs[0].second.Object = &defFlags;
    defPairs[1].first = ANIM_COMPONENT_TRANSITION_IN_TIME; defPairs[1].second.Object = 0;
    defPairs[2].first = (EAnimComponent)99; defPairs[2].second.Object = &sentinel;
    SetMap(g_DefaultAnimComponents.Map, defPairs, 2);

    CAnimationEntry entry; memset(&entry, 0, sizeof(entry));
    const CAnimComponentFlags* out = 0;

    // no component map at all -> default
    entry.Components.Object = 0;
    entry.GetComponent(&out);
    if (out != &defFlags) ok = 0;

    // own map holds type 0 -> own
    CAnimComponentMap own;
    TPair ownPairs[3];
    ownPairs[0].first = ANIM_COMPONENT_FLAGS; ownPairs[0].second.Object = &ownFlags;
    ownPairs[1].first = ANIM_COMPONENT_TRANSITION_IN_TIME; ownPairs[1].second.Object = 0;
    ownPairs[2].first = (EAnimComponent)99; ownPairs[2].second.Object = 0;
    SetMap(own.Map, ownPairs, 2);
    entry.Components.Object = &own;
    out = 0;
    entry.GetComponent(&out);
    if (out != &ownFlags) ok = 0;

    // own map lacks type 0 (holds only type 1) -> default
    SetMap(own.Map, ownPairs + 1, 1);
    out = 0;
    entry.GetComponent(&out);
    if (out != &defFlags) ok = 0;

    // own map empty -> default
    SetMap(own.Map, ownPairs, 0);
    out = 0;
    entry.GetComponent(&out);
    if (out != &defFlags) ok = 0;

    // default lacks type 0 too -> the default map's end()->second (unchecked)
    SetMap(g_DefaultAnimComponents.Map, defPairs + 1, 1);
    out = 0;
    entry.GetComponent(&out);
    if (out != &sentinel) ok = 0;

    printf("005e0380_TEST %s\n", ok ? "PASS" : "FAIL");
    return ok ? 0 : 1;
}