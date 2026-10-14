// Behaviour test for CAnimationSet::CAnimationSet @ 0x005dd2e0: empty map (not dirty), zero defaults, default group -1.
#include <cstdio>
#include <cstddef>
struct CKeyPairCompareLess { };
template<class K, class V> struct CVectorMap {
    struct SPair { K first; V second; };
    SPair* m_first; SPair* m_last; SPair* m_end; CKeyPairCompareLess Compare; bool Dirty;
    CVectorMap() : m_first(0), m_last(0), m_end(0), Dirty(false) { }
};
struct CDefString { long TablePos; CDefString() : TablePos(-1) { } };
struct CAnimationEntry { long BankIndex; CDefString AnimName; CDefString GroupName; void* Components; };
struct CAnimationSet {
    CVectorMap<unsigned long, CAnimationEntry> Anims; long DefaultFlags; long DefaultTransitionTime; long DefaultDelay; CDefString DefaultGroup;
    CAnimationSet() : DefaultFlags(0), DefaultTransitionTime(0), DefaultDelay(0) { }
};
typedef CVectorMap<unsigned long, CAnimationEntry> TAnimMap;
int main()
{
    CAnimationSet s;
    bool ok = s.Anims.m_first == 0 && s.Anims.m_last == 0 && s.Anims.m_end == 0 && !s.Anims.Dirty && s.DefaultFlags == 0
        && s.DefaultTransitionTime == 0 && s.DefaultDelay == 0 && s.DefaultGroup.TablePos == -1 && sizeof(s) == 0x20
        && offsetof(CAnimationSet, DefaultGroup) == 0x1c && offsetof(TAnimMap, Dirty) == 0x0d;
    if (!ok) { std::printf("005dd2e0_TEST FAIL\n"); return 1; }
    std::printf("005dd2e0_TEST PASS\n");
    return 0;
}