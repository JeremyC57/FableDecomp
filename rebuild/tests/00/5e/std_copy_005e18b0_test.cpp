// behaviour test for 005e18b0: STLport random-access __copy of (CRC, CAnimationEntry) pairs -- assigns
// [first, last) onto result, returns the end of the output, and keeps the component maps' intrusive counts right.
#include <cstdio>
static int g_destroyed = 0;
struct CIVCountedPointeeBase {
    virtual void v00() { }
    virtual void Destroy() { ++g_destroyed; }   // slot 1
    long m_count;
    CIVCountedPointeeBase() : m_count(0) { }
    virtual ~CIVCountedPointeeBase() { }
};
template <class T>
struct CIVCountedPointer {
    T* Object;
    CIVCountedPointer() : Object(0) { }
    CIVCountedPointer& operator=(const CIVCountedPointer& other)
    {
        if (Object != other.Object) {
            T* object = Object;
            if (object) {
                if (--object->m_count == 0)
                    object->Destroy();
                Object = 0;
            }
            Object = other.Object;
            if (Object)
                ++Object->m_count;
        }
        return *this;
    }
    void Attach(T* p) { Object = p; ++p->m_count; }
};
struct CDefString { long TablePos; };
struct CAnimComponentMap : CIVCountedPointeeBase { };
struct CAnimationEntry {
    long                                 BankIndex;
    CDefString                           AnimName;
    CDefString                           GroupName;
    CIVCountedPointer<CAnimComponentMap> Components;
};
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
};
struct random_access_iterator_tag { };
template <class RandomAccessIter, class OutputIter, class Distance>
OutputIter __copy(RandomAccessIter first, RandomAccessIter last, OutputIter result, const random_access_iterator_tag&, Distance*)
{
    for (Distance n = last - first; n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}
typedef pair<unsigned long, CAnimationEntry> CAnimationEntryPair;
static CAnimComponentMap g_maps[4];
int main()
{
    CAnimationEntryPair src[4];
    CAnimationEntryPair dst[6];
    int i = 0;
    int ok = 1;
    for (i = 0; i < 4; ++i) {
        src[i].first = 0x1000u + i;
        src[i].second.BankIndex = i + 1;
        src[i].second.AnimName.TablePos = i * 10;
        src[i].second.GroupName.TablePos = i * 20;
        src[i].second.Components.Attach(&g_maps[i]);
    }
    for (i = 0; i < 6; ++i) {
        dst[i].first = 0;
        dst[i].second.BankIndex = -1;
        dst[i].second.AnimName.TablePos = -1;
        dst[i].second.GroupName.TablePos = -1;
    }
    dst[0].second.Components.Attach(&g_maps[3]);       // overwritten: map 3 loses this reference
    random_access_iterator_tag tag;
    CAnimationEntryPair* end = __copy(src, src + 4, dst, tag, (int*)0);
    if (end != dst + 4) ok = 0;
    for (i = 0; i < 4; ++i) {
        if (dst[i].first != 0x1000u + i || dst[i].second.BankIndex != i + 1 || dst[i].second.AnimName.TablePos != i * 10
            || dst[i].second.GroupName.TablePos != i * 20 || dst[i].second.Components.Object != &g_maps[i])
            ok = 0;
        if (g_maps[i].m_count != 2) ok = 0;            // the source and the copy
    }
    if (dst[4].second.BankIndex != -1 || dst[4].second.Components.Object != 0) ok = 0;
    if (g_destroyed != 0) ok = 0;
    // an empty range copies nothing and returns result
    if (__copy(src, src, dst + 5, tag, (int*)0) != dst + 5 || dst[5].second.BankIndex != -1) ok = 0;
    // reassignment moves one reference from the old map to the new one
    dst[4].second.Components.Attach(&g_maps[0]);
    for (i = 0; i < 4; ++i) src[i].second.Components = CIVCountedPointer<CAnimComponentMap>();
    if (g_maps[0].m_count != 2 || g_maps[1].m_count != 1) ok = 0;
    __copy(dst + 1, dst + 2, dst + 4, tag, (int*)0);   // map 0: 2 -> 1, map 1: 1 -> 2
    if (g_maps[0].m_count != 1 || g_maps[1].m_count != 2 || dst[4].first != 0x1001u || g_destroyed != 0) ok = 0;
    if (!ok) { std::printf("005e18b0_TEST FAIL\n"); return 1; }
    std::printf("005e18b0_TEST PASS\n");
    return 0;
}