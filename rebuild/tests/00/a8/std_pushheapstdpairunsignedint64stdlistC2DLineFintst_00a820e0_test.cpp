// Behaviour test for 0x00A820E0 std::__push_heap<pair<unsigned __int64, list<C2DLineF> >*, int, pair<..>, CKeyPairCompareLess<..> > on
// the shared headers (the row's macro set): every push keeps a max-heap on the UNSIGNED 64-bit key (keys chosen so that a signed, a
// low-dword-only or a high-dword-only compare order them differently), each slot's list is a deep copy that identifies its key, and
// the by-value element is destroyed (no shared nodes). list<C2DLineF>::operator= (out of line in retail, 0x00517720) is supplied here
// as a counting deep copy.
#define FABLE_RETAIL_STLP_DECAL_HEAP
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_INSERT_DISPATCH_NOINLINE
#define FABLE_RETAIL_STLP_LIST_BASE_DTOR_VISIBLE_NOINLINE
#define FABLE_RETAIL_STLP_LIST_ASSIGN_POPBACK_DECL
#include "stlport/fable_containers.h"
#include "engine/C2DLineF.h"
#include "engine/CVectorMap.h"
#include <stdio.h>
typedef std::list<C2DLineF> L;
typedef std::pair<unsigned __int64, L> Elem;
typedef CKeyPairCompareLess<unsigned __int64, L> Less;
static int g_assigns = 0;
namespace std {
template <> list<C2DLineF>& list<C2DLineF>::operator=(const list<C2DLineF>& x)
{
    ++g_assigns;
    if (this != &x) {
        clear();
        for (const_iterator it = x.begin(); it != x.end(); ++it) push_back(*it);
    }
    return *this;
}
}
static void fill(L& l, int id) { for (int i = 0; i <= id % 3; ++i) { C2DLineF f; f.Point1.X = (float)id; f.Point1.Y = (float)i; f.Point2.X = 0.0f; f.Point2.Y = 0.0f; l.push_back(f); } }
static bool same(const L& l, int id)
{
    int i = 0;
    for (L::const_iterator it = l.begin(); it != l.end(); ++it, ++i)
        if (it->Point1.X != (float)id || it->Point1.Y != (float)i) return false;
    return i == id % 3 + 1;
}
int main()
{
    unsigned __int64 keys[8] = { 9, ((unsigned __int64)3 << 32), 0xFFFFFFFFu, ((unsigned __int64)3 << 32) + 7, 1, 0,
                                 ((unsigned __int64)1 << 33), ((unsigned __int64)0x80000000u << 32) };
    Elem h[8];
    int id[8];
    for (int n = 0; n < 8; ++n) {
        Elem v;
        v.first = keys[n];
        fill(v.second, n);
        h[n].first = 0;
        std::__push_heap(h, n, 0, v, Less());
        for (int c = 1; c <= n; ++c)
            if (h[(c - 1) / 2].first < h[c].first) { printf("00a820e0_TEST FAIL heap %d\n", n); return 1; }
    }
    for (int s = 0; s < 8; ++s) {
        int k = -1;
        for (int j = 0; j < 8; ++j) if (keys[j] == h[s].first) k = j;
        if (k < 0 || !same(h[s].second, k)) { printf("00a820e0_TEST FAIL slot %d\n", s); return 1; }
        id[s] = k;
        for (int t = 0; t < s; ++t)
            if (h[t].second._M_node._M_data->_M_next == h[s].second._M_node._M_data->_M_next) { printf("00a820e0_TEST FAIL shared\n"); return 1; }
    }
    if (id[0] != 7 || g_assigns < 8) { printf("00a820e0_TEST FAIL root %d %d\n", id[0], g_assigns); return 1; }
    printf("00a820e0_TEST PASS\n");
    return 0;
}
