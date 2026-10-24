// Behaviour test for 0x0067C8B0 std::__copy_backward<CBlurredAttitudesDef*, CBlurredAttitudesDef*, int>: every def of the
// range is assigned memberwise (root words, manager, index, the three flag bits, instantiation name, the sub-def map's
// vector + Dirty byte, the attitudes vector) back to front, returning the moved-back result.
#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#endif

static int g_assigns = 0;

namespace nav {
template <class T> class vector {
public:
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    vector<T>& operator=(const vector<T>& x)                // models 0x00431F90 / 0x00471E44 (pointer copy)
    {
        ++g_assigns; _M_start = x._M_start; _M_finish = x._M_finish; _M_end_of_storage = x._M_end_of_storage;
        return *this;
    }
};
template <class T1, class T2> class pair { public: T1 first; T2 second; };
struct random_access_iterator_tag { };
}

template <class K, class V> class CKeyPairCompareLess { };
template <class K, class V, class C> class CVectorMap {
public:
    nav::vector<nav::pair<K, V> > Data;
    C    Comparator;
    bool Dirty;
};
class CResource {
public:
    virtual ~CResource() {}
    int m_words[6];
};
class CDefClassBase : public CResource {
public:
    class CSubDefInfo { public: int m_0; int m_4; };
    void*         PDefManager;
    unsigned int  GlobalIndex;
    unsigned char Setup : 1;
    unsigned char Template : 1;
    unsigned char DefaultValsApplied : 1;
};
class CParentDefClassBase : public CDefClassBase {
public:
    int InstantiationName;
    CVectorMap<unsigned int, CDefClassBase::CSubDefInfo, CKeyPairCompareLess<unsigned int, CDefClassBase::CSubDefInfo> > SubDefInfoMap;
};
enum EOpinionAttitudeType { OPINION_ATTITUDE_NONE = -1, OPINION_ATTITUDE_0 = 0 };
class CBlurredAttitudesDef : public CParentDefClassBase {
public:
    nav::vector<EOpinionAttitudeType> Attitudes;
};

namespace nav {
template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
_BidirectionalIter __fastcall __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last,
                                              _BidirectionalIter __result, const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}
}

static void Fill(CBlurredAttitudesDef& d, int seed)
{
    for (int i = 0; i < 6; ++i) d.m_words[i] = seed * 10 + i;
    d.PDefManager = (void*)0; d.GlobalIndex = seed + 100;
    d.Setup = seed & 1; d.Template = (seed >> 1) & 1; d.DefaultValsApplied = (seed >> 2) & 1;
    d.InstantiationName = seed + 200;
    d.SubDefInfoMap.Data._M_start = d.SubDefInfoMap.Data._M_finish = d.SubDefInfoMap.Data._M_end_of_storage = 0;
    d.SubDefInfoMap.Dirty = (seed & 1) != 0;
    d.Attitudes._M_start = d.Attitudes._M_finish = d.Attitudes._M_end_of_storage = 0;
}
static bool Same(const CBlurredAttitudesDef& d, int seed)
{
    for (int i = 0; i < 6; ++i) if (d.m_words[i] != seed * 10 + i) return false;
    return d.GlobalIndex == (unsigned int)(seed + 100) && d.Setup == (seed & 1) && d.Template == ((seed >> 1) & 1) &&
           d.DefaultValsApplied == ((seed >> 2) & 1) && d.InstantiationName == seed + 200 &&
           d.SubDefInfoMap.Dirty == ((seed & 1) != 0);
}

int main()
{
    CBlurredAttitudesDef a[5];
    for (int i = 0; i < 5; ++i) Fill(a[i], i + 1);
    // overlapping move up: [a0, a3) -> ends at a4, the backward copy is the safe direction
    CBlurredAttitudesDef* begin = nav::__copy_backward(a, a + 3, a + 4, nav::random_access_iterator_tag(), (int*)0);
    if (begin != a + 1 || g_assigns != 6 || !Same(a[0], 1) || !Same(a[1], 1) || !Same(a[2], 2) || !Same(a[3], 3) ||
        !Same(a[4], 5)) { std::printf("0067c8b0_TEST FAIL\n"); return 1; }
    if (nav::__copy_backward(a, a, a + 2, nav::random_access_iterator_tag(), (int*)0) != a + 2 || g_assigns != 6) {
        std::printf("0067c8b0_TEST FAIL\n"); return 1;
    }
    std::printf("0067c8b0_TEST PASS\n");
    return 0;
}