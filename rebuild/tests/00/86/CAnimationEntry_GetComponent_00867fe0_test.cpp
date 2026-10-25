// Behaviour test for 0x00867FE0 CAnimationEntry::GetComponent<CAnimComponentFollowingOnAnim>(const T**) const: the entry's
// own type-8 component when its component map has one, else the default map's (its end()->second when absent).
#include <stdio.h>
#include <string.h>
#ifndef _MSC_VER
#define __forceinline inline
#define __declspec(x)
#endif
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
    CAnimComponentFollowingOnAnim own8, def8, sentinel;
    int ok = 1;

    // default map: {3 -> null, 8 -> def8}, then a sentinel slot past end
    TPair defPairs[3];
    defPairs[0].first = (EAnimComponent)3; defPairs[0].second.Object = 0;
    defPairs[1].first = ANIM_COMPONENT_FOLLOWING_ON_ANIM; defPairs[1].second.Object = &def8;
    defPairs[2].first = (EAnimComponent)99; defPairs[2].second.Object = &sentinel;
    SetMap(g_DefaultAnimComponents.Map, defPairs, 2);

    CAnimationEntry entry; memset(&entry, 0, sizeof(entry));
    const CAnimComponentFollowingOnAnim* out = 0;

    entry.Components.Object = 0;                 // no component map -> default
    entry.GetComponent(&out);
    if (out != &def8) ok = 0;

    CAnimComponentMap own;
    TPair ownPairs[3];
    ownPairs[0].first = (EAnimComponent)2; ownPairs[0].second.Object = 0;
    ownPairs[1].first = ANIM_COMPONENT_FOLLOWING_ON_ANIM; ownPairs[1].second.Object = &own8;
    ownPairs[2].first = (EAnimComponent)9; ownPairs[2].second.Object = 0;
    SetMap(own.Map, ownPairs, 3);
    entry.Components.Object = &own;              // own map holds type 8 -> own
    out = 0;
    entry.GetComponent(&out);
    if (out != &own8) ok = 0;

    SetMap(own.Map, ownPairs + 2, 1);            // own map holds only type 9 -> default
    out = 0;
    entry.GetComponent(&out);
    if (out != &def8) ok = 0;

    SetMap(own.Map, ownPairs, 0);                // own map empty -> default
    out = 0;
    entry.GetComponent(&out);
    if (out != &def8) ok = 0;

    defPairs[1].first = (EAnimComponent)5;       // default lacks type 8 too -> end()->second (unchecked)
    out = 0;
    entry.GetComponent(&out);
    if (out != &sentinel) ok = 0;

    printf("00867fe0_TEST %s\n", ok ? "PASS" : "FAIL");
    return ok ? 0 : 1;
}