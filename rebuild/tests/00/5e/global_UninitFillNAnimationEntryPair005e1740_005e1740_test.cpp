// Behaviour test for 0x005E1740 UninitFillN_AnimationEntryPair_005e1740: fill n pair<unsigned long, CAnimationEntry>
// (the component map gains one intrusive reference per copy).
#include <cstdio>
#include <new>

struct __false_type { __false_type() { } };
class CAnimComponentMap { public: long m_vtablePlaceholder; long m_count; CAnimComponentMap() : m_vtablePlaceholder(0), m_count(1) { } };
struct CIVCountedPointer_CAnimComponentMap {
    CAnimComponentMap* Object;
    CIVCountedPointer_CAnimComponentMap() : Object(0) { }
    CIVCountedPointer_CAnimComponentMap(const CIVCountedPointer_CAnimComponentMap& x) : Object(x.Object) { if (Object) ++Object->m_count; }
};
class CDefString { public: long TablePos; };
class CAnimationEntry { public:
    long                                BankIndex;
    CDefString                          AnimName;
    CDefString                          GroupName;
    CIVCountedPointer_CAnimComponentMap Components;
};
struct CAnimationEntryPair {
    unsigned long   first;
    CAnimationEntry second;
    CAnimationEntryPair() : first(0) { second.BankIndex = 0; second.AnimName.TablePos = 0; second.GroupName.TablePos = 0; }
    CAnimationEntryPair(const CAnimationEntryPair& o) : first(o.first), second(o.second) { }
};

static CAnimationEntryPair* UninitFillN(CAnimationEntryPair* first, unsigned int n, const CAnimationEntryPair& x, const __false_type&)
{
    CAnimationEntryPair* cur = first;
    for (; n > 0; --n, ++cur)
        new (cur) CAnimationEntryPair(x);
    return cur;
}

int main()
{
    CAnimComponentMap map;
    CAnimationEntryPair x;
    x.first = 0xABCDu; x.second.BankIndex = 3; x.second.AnimName.TablePos = 17; x.second.GroupName.TablePos = 29;
    x.second.Components.Object = &map;
    CAnimationEntryPair dst[3];
    __false_type tag;
    CAnimationEntryPair* r = UninitFillN(dst, 3, x, tag);
    bool ok = r == dst + 3 && map.m_count == 4;
    for (int k = 0; k < 3; ++k)
        if (dst[k].first != 0xABCDu || dst[k].second.BankIndex != 3 || dst[k].second.AnimName.TablePos != 17
            || dst[k].second.GroupName.TablePos != 29 || dst[k].second.Components.Object != &map) ok = false;
    if (UninitFillN(dst, 0, x, tag) != dst || map.m_count != 4) ok = false;
    if (!ok) { std::printf("005e1740_TEST FAIL\n"); return 1; }
    std::printf("005e1740_TEST PASS\n");
    return 0;
}