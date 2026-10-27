// behaviour test for 0x0095CDF0 std::__push_heap over CThing* with a stand-in comparator of the same by-value layout
#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#endif
class CThing { public: int key; };
class CThingObject : public CThing { };
struct TComp { int pad[3]; bool operator()(CThing* a, CThing* b) const { return a->key < b->key; } };   // max-heap on key
namespace std {
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __push_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __topIndex, _Tp __val,
                            _Compare __comp)
{
    _Distance __parent = (__holeIndex - 1) / 2;
    while (__holeIndex > __topIndex && __comp(*(__first + __parent), __val)) {
        *(__first + __holeIndex) = *(__first + __parent);
        __holeIndex = __parent;
        __parent = (__holeIndex - 1) / 2;
    }
    *(__first + __holeIndex) = __val;
}
}
int main()
{
    CThingObject t[6];
    int keys[6] = { 9, 7, 8, 3, 4, 10 };
    for (int i = 0; i < 6; ++i) t[i].key = keys[i];
    CThing* heap[6] = { &t[0], &t[1], &t[2], &t[3], &t[4], 0 };   // a valid max-heap of five
    TComp comp;
    std::__push_heap(heap, 5, 0, (CThing*)&t[5], comp);         // 10 rises to the root
    int ok = heap[0] == &t[5] && heap[2] == &t[0] && heap[5] == &t[2] && heap[1] == &t[1];
    std::__push_heap(heap, 1, 1, (CThing*)&t[3], comp);         // hole == top: stored in place
    ok = ok && heap[1] == &t[3];
    if (!ok) { std::printf("0095cdf0_TEST FAIL\n"); return 1; }
    std::printf("0095cdf0_TEST PASS\n");
    return 0;
}