// Behaviour test for 0x0086B2A0: the STLport vector copy constructor allocates exactly size() elements and copies them;
// an empty source gives null pointers.
#include <cstdio>
#include <cstdlib>
#include <new>

struct CMorphSetTextureMorph { int OriginalTexture; int NewTexture; };

struct SVector
{
    CMorphSetTextureMorph* _M_start;
    CMorphSetTextureMorph* _M_finish;
    CMorphSetTextureMorph* _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    SVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    SVector(const SVector& x) : _M_start(0), _M_finish(0), _M_end_of_storage(0)
    {
        unsigned int n = x.size();
        _M_start = n != 0 ? (CMorphSetTextureMorph*)std::malloc(n * sizeof(CMorphSetTextureMorph)) : 0;
        _M_finish = _M_start;
        _M_end_of_storage = _M_start + n;
        CMorphSetTextureMorph* cur = _M_start;
        for (const CMorphSetTextureMorph* p = x._M_start; p != x._M_finish; ++p, ++cur)
            new (cur) CMorphSetTextureMorph(*p);
        _M_finish = cur;
    }
};

int main()
{
    CMorphSetTextureMorph src[3] = { { 1, 2 }, { 3, 4 }, { 5, 6 } };
    SVector a; a._M_start = src; a._M_finish = src + 3; a._M_end_of_storage = src + 3;
    int fails = 0;
    if (sizeof(CMorphSetTextureMorph) != 8u) ++fails;
    SVector b(a);
    if (b.size() != 3u || b._M_end_of_storage != b._M_start + 3 || b._M_start == src) ++fails;
    for (int i = 0; i < 3; ++i) if (b._M_start[i].OriginalTexture != src[i].OriginalTexture || b._M_start[i].NewTexture != src[i].NewTexture) ++fails;
    SVector e;
    SVector f(e);
    if (f._M_start != 0 || f._M_finish != 0 || f._M_end_of_storage != 0) ++fails;
    std::free(b._M_start);
    if (fails) { std::printf("0086b2a0_TEST FAIL\n"); return 1; }
    std::printf("0086b2a0_TEST PASS\n");
    return 0;
}