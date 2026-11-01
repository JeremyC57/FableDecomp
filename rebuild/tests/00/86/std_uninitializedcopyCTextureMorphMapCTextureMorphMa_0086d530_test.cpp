// Behaviour test for std::__uninitialized_copy<CTextureMorphMap*, CTextureMorphMap*> 0x0086D530: copy-construct each map of
// [first, last) into raw storage (vector copy + Dirty), return the end of the copied range
#include <cstdio>
#include <cstdlib>
#include <new>
static int g_vectorCopies = 0;
struct TextureMorphPair { unsigned long first; unsigned long NewTexture; unsigned long GroupID; };
struct PairVector {
    TextureMorphPair* _M_start;
    TextureMorphPair* _M_finish;
    TextureMorphPair* _M_end_of_storage;
    PairVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    PairVector(const PairVector& x)
    {
        ++g_vectorCopies;
        unsigned int n = (unsigned int)(x._M_finish - x._M_start);
        _M_start = n ? (TextureMorphPair*)std::malloc(n * sizeof(TextureMorphPair)) : 0;
        for (unsigned int i = 0; i < n; ++i) _M_start[i] = x._M_start[i];
        _M_finish = _M_start + n;
        _M_end_of_storage = _M_finish;
    }
};
struct CKeyPairCompareLess { };
struct CTextureMorphMap : public PairVector {
    CKeyPairCompareLess Compare;
    bool Dirty;
};
struct __false_type { };
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _InputIter, class _ForwardIter>
_ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for ( ; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
int main()
{
    TextureMorphPair pairs[3] = { { 1, 10, 100 }, { 2, 20, 200 }, { 3, 30, 300 } };
    CTextureMorphMap source[2];
    source[0]._M_start = pairs; source[0]._M_finish = pairs + 2; source[0]._M_end_of_storage = pairs + 2; source[0].Dirty = true;
    source[1]._M_start = pairs + 2; source[1]._M_finish = pairs + 3; source[1]._M_end_of_storage = pairs + 3; source[1].Dirty = false;
    static double raw[2][(sizeof(CTextureMorphMap) + sizeof(double) - 1) / sizeof(double)];
    CTextureMorphMap* dest = (CTextureMorphMap*)raw;
    CTextureMorphMap* end = __uninitialized_copy(source, source + 2, dest, __false_type());
    if (end != dest + 2 || g_vectorCopies != 2) { std::printf("0086d530_TEST FAIL\n"); return 1; }
    if (dest[0]._M_finish - dest[0]._M_start != 2 || dest[0]._M_start[1].NewTexture != 20 || dest[0].Dirty != true) {
        std::printf("0086d530_TEST FAIL\n");
        return 1;
    }
    if (dest[1]._M_finish - dest[1]._M_start != 1 || dest[1]._M_start[0].GroupID != 300 || dest[1].Dirty != false) {
        std::printf("0086d530_TEST FAIL\n");
        return 1;
    }
    if (__uninitialized_copy(source, source, dest, __false_type()) != dest || g_vectorCopies != 2) {
        std::printf("0086d530_TEST FAIL\n");
        return 1;
    }
    std::free(dest[0]._M_start);
    std::free(dest[1]._M_start);
    std::printf("0086d530_TEST PASS\n");
    return 0;
}