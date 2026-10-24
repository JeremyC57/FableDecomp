// Behaviour test for 0x0086D5A0 _STL::__copy_backward over 16-byte CVectorMap records (vector assignment out of line, empty comparator, dirty flag).
#include <cstdio>

static int g_assigns = 0;
struct SVector_006741e0 {
    void* m_first; void* m_last; void* m_end;
    SVector_006741e0& operator=(const SVector_006741e0& other)   // test stand-in for 0x006741E0
    {
        ++g_assigns;
        m_first = other.m_first; m_last = other.m_last; m_end = other.m_end;
        return *this;
    }
};
struct CKeyPairCompareLess { };
struct SVectorMap {
    SVector_006741e0    Vector;
    CKeyPairCompareLess Compare;
    bool                Dirty;
};
struct random_access_iterator_tag { random_access_iterator_tag() { } };

static SVectorMap* __copy_backward(SVectorMap* first, SVectorMap* last, SVectorMap* result, const random_access_iterator_tag&, int*)
{
    for (int n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}

int main()
{
    static int storage[16];
    SVectorMap a[6];
    for (int k = 0; k < 6; ++k) {
        a[k].Vector.m_first = &storage[k]; a[k].Vector.m_last = &storage[k + 1]; a[k].Vector.m_end = &storage[k + 2];
        a[k].Dirty = (k % 2) == 0;
    }
    // overlapping move up by two (the vector insert case): [0,4) -> [2,6)
    SVectorMap* r = __copy_backward(a, a + 4, a + 6, random_access_iterator_tag(), (int*)0);
    if (r != a + 2 || g_assigns != 4) { std::printf("0086d5a0_TEST FAIL\n"); return 1; }
    for (int k = 2; k < 6; ++k)
        if (a[k].Vector.m_first != &storage[k - 2] || a[k].Vector.m_end != &storage[k] || a[k].Dirty != (((k - 2) % 2) == 0))
            { std::printf("0086d5a0_TEST FAIL\n"); return 1; }
    if (__copy_backward(a, a, a + 3, random_access_iterator_tag(), (int*)0) != a + 3 || g_assigns != 4) { std::printf("0086d5a0_TEST FAIL\n"); return 1; }
    std::printf("0086d5a0_TEST PASS\n");
    return 0;
}