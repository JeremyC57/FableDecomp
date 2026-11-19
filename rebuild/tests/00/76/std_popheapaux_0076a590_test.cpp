// Behaviour test for 0x0076A590 std::__pop_heap_aux<pair<unsigned long, C3DVector>*, ..., CParticleUpdateCCompareVector<unsigned long> >:
// the row's defines, includes and instantiation verbatim, with a reference STLport __adjust_heap (the out-of-line callee 0x00769D50) defined
// here. Popping a max-heap built by hand must move the largest key to the back slot (with its vector), keep [first, last-1) a heap, and
// carry the old back element's vector with its key; repeated pops give the keys in descending order (unsigned compare: 0x80000000 is large).
#include <stdio.h>
#define FABLE_RETAIL_STLP_HEAP
#define FABLE_RETAIL_STLP_POP_HEAP_AUX_PASS22
#include "stlport/fable_containers.h"
#include "engine/NParticleEngine_CParticleUpdateParameters.h"

typedef std::pair<unsigned long, C3DVector> TVectorParam;
typedef NParticleEngine::CParticleUpdateParameters<unsigned long, unsigned long>::CParticleUpdateCCompareVector<unsigned long> TVectorParamCompare;
template void FABLE_STLP_FASTCALL std::__pop_heap_aux<TVectorParam*, TVectorParam, TVectorParamCompare>(TVectorParam*, TVectorParam*, TVectorParam*, TVectorParamCompare);

namespace std {
template<class _RandomAccessIterator,class _Distance,class _Tp,class _Compare>
void FABLE_STLP_FASTCALL __adjust_heap(_RandomAccessIterator __first,_Distance __holeIndex,_Distance __len,_Tp __val,_Compare __comp)
{
    _Distance __topIndex = __holeIndex;
    _Distance __secondChild = 2 * __holeIndex + 2;
    while (__secondChild < __len) {
        if (__comp(*(__first + __secondChild), *(__first + (__secondChild - 1))))
            __secondChild--;
        *(__first + __holeIndex) = *(__first + __secondChild);
        __holeIndex = __secondChild;
        __secondChild = 2 * (__secondChild + 1);
    }
    if (__secondChild == __len) {
        *(__first + __holeIndex) = *(__first + (__secondChild - 1));
        __holeIndex = __secondChild - 1;
    }
    _Distance __parent = (__holeIndex - 1) / 2;
    while (__holeIndex > __topIndex && __comp(*(__first + __parent), __val)) {
        *(__first + __holeIndex) = *(__first + __parent);
        __holeIndex = __parent;
        __parent = (__holeIndex - 1) / 2;
    }
    *(__first + __holeIndex) = __val;
}
}
static int g_fail = 0;
static void check(int ok, const char* what) { if (!ok) { printf("FAIL %s\n", what); g_fail = 1; } }
static TVectorParam P(unsigned long k) { TVectorParam p; p.first = k; p.second.X = (float)(k & 0xff) + 0.5f; p.second.Y = -(float)(k & 0xff); p.second.Z = 2.0f * (float)(k & 0xff); return p; }
static int Same(const TVectorParam& p, unsigned long k) { TVectorParam q = P(k); return p.first == k && p.second.X == q.second.X && p.second.Y == q.second.Y && p.second.Z == q.second.Z; }
int main()
{
    // a max-heap of 7 keys (unsigned): 0x80000001 at the root
    unsigned long keys[7] = { 0x80000001ul, 50, 0x80000000ul, 10, 40, 30, 20 };
    TVectorParam h[7];
    for (int i = 0; i < 7; ++i) h[i] = P(keys[i]);
    unsigned long expect[7] = { 0x80000001ul, 0x80000000ul, 50, 40, 30, 20, 10 };
    for (int n = 7; n >= 2; --n) {
        std::__pop_heap_aux(h, h + n, (TVectorParam*)0, TVectorParamCompare());
        check(Same(h[n - 1], expect[7 - n]), "largest key (unsigned) with its vector at the back");
        for (int c = 1; c < n - 1; ++c)
            check(!(h[(c - 1) / 2].first < h[c].first), "the rest stays a heap");
    }
    check(Same(h[0], 10), "smallest left at the front");
    // the old back element keeps its vector when it is sifted down
    TVectorParam g[3]; g[0] = P(9); g[1] = P(3); g[2] = P(7);
    std::__pop_heap_aux(g, g + 3, (TVectorParam*)0, TVectorParamCompare());
    check(Same(g[2], 9) && Same(g[0], 7) && Same(g[1], 3), "three-element pop");
    if (!g_fail) printf("0076a590_TEST PASS\n");
    return g_fail;
}