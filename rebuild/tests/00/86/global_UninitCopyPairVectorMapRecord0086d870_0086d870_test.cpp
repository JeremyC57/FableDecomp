// Behaviour test for 0x0086D870 UninitCopy_PairVectorMapRecord_0086d870: placement copy of {long, long, CVectorMap} records.
#include <cstdio>
#include <new>

struct __false_type { __false_type() { } };
static int g_vectorCopies = 0;
struct SVector {
    void* _M_start;
    void* _M_finish;
    void* _M_end_of_storage;
    SVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    SVector(const SVector& other) : _M_start(other._M_start), _M_finish(other._M_finish), _M_end_of_storage(other._M_end_of_storage) { ++g_vectorCopies; }
};
struct SCompare { };
struct SVectorMap : SVector {
    SCompare Compare;
    bool     Dirty;
    SVectorMap() : Dirty(false) { }
};
struct SRecord24 { long m_0; long m_4; SVectorMap m_map; };

static SRecord24* UninitCopy(const SRecord24* first, const SRecord24* last, SRecord24* result, const __false_type&)
{
    SRecord24* cur = result;
    for (; first != last; ++first, ++cur)
        new (cur) SRecord24(*first);
    return cur;
}

int main()
{
    static int marks[3];
    SRecord24 src[3];
    for (int i = 0; i < 3; ++i) { src[i].m_0 = i; src[i].m_4 = 100 + i; src[i].m_map._M_start = &marks[i]; src[i].m_map.Dirty = (i == 1); }
    SRecord24 dst[3];
    __false_type tag;
    SRecord24* r = UninitCopy(src, src + 3, dst, tag);
    bool ok = r == dst + 3 && g_vectorCopies == 3;
    for (int k = 0; k < 3; ++k)
        if (dst[k].m_0 != k || dst[k].m_4 != 100 + k || dst[k].m_map._M_start != &marks[k] || dst[k].m_map.Dirty != (k == 1)) ok = false;
    if (UninitCopy(src, src, dst, tag) != dst || g_vectorCopies != 3) ok = false;
    if (!ok) { std::printf("0086d870_TEST FAIL\n"); return 1; }
    std::printf("0086d870_TEST PASS\n");
    return 0;
}