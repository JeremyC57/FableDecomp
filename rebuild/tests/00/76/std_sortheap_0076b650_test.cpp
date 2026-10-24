// Behaviour test for 0076b650: SGI sort_heap over (unsigned long, CParticleUpdateParamDesc) pairs -- repeatedly pop the root to the back (via __adjust_heap) until the range is sorted ascending by key
#include <cstdio>

class CBaseClass {
public:
    virtual ~CBaseClass() { }
};
class CParticleUpdateParamDesc : public CBaseClass {
public:
    CParticleUpdateParamDesc() : Callback(0), Index(0) { }
    virtual int Kind() const { return 0xEE4; }
    void*        Callback;
    unsigned int Index;
};
struct PairULongParamDesc {
    unsigned int             first;
    CParticleUpdateParamDesc second;
    PairULongParamDesc() : first(0) { }
    PairULongParamDesc(const PairULongParamDesc& other) : first(other.first), second(other.second) { }
};
struct CCompare {
    bool operator()(const PairULongParamDesc& a, const PairULongParamDesc& b) const { return a.first < b.first; }
};

// copies of the out-of-line heap helpers (retail __adjust_heap 0x0076A4E0 and its __push_heap)
static void PushHeap(PairULongParamDesc* first, long holeIndex, long topIndex, PairULongParamDesc value, CCompare comp)
{
    long parent = (holeIndex - 1) / 2;
    while (holeIndex > topIndex && comp(first[parent], value)) {
        first[holeIndex] = first[parent];
        holeIndex = parent;
        parent = (holeIndex - 1) / 2;
    }
    first[holeIndex] = value;
}
static void AdjustHeap(PairULongParamDesc* first, long holeIndex, long len, PairULongParamDesc value, CCompare comp)
{
    long topIndex = holeIndex;
    long secondChild = 2 * holeIndex + 2;
    while (secondChild < len) {
        if (comp(first[secondChild], first[secondChild - 1]))
            secondChild--;
        first[holeIndex] = first[secondChild];
        holeIndex = secondChild;
        secondChild = 2 * (secondChild + 1);
    }
    if (secondChild == len) {
        first[holeIndex] = first[secondChild - 1];
        holeIndex = secondChild - 1;
    }
    PushHeap(first, holeIndex, topIndex, value, comp);
}
inline void PopHeapCore(PairULongParamDesc* first, PairULongParamDesc* last, PairULongParamDesc* result, PairULongParamDesc value, CCompare comp)
{
    *result = *first;
    AdjustHeap(first, 0, long(last - first), value, comp);
}
inline void PopHeap(PairULongParamDesc* first, PairULongParamDesc* last, CCompare comp)
{
    PopHeapCore(first, last - 1, last - 1, PairULongParamDesc(*(last - 1)), comp);
}
static void SortHeap(PairULongParamDesc* first, PairULongParamDesc* last, CCompare comp)
{
    while (last - first > 1)
        PopHeap(first, last--, comp);
}

int main()
{
    unsigned int keys[9] = { 40, 7, 0xFFFFFFF0u, 13, 7, 99, 1, 0x80000000u, 25 };
    PairULongParamDesc v[9];
    for (int i = 0; i < 9; ++i) {
        v[i].first = keys[i];
        v[i].second.Index = 1000 + i;
        PushHeap(v, i, 0, PairULongParamDesc(v[i]), CCompare());   // make_heap by successive pushes
    }
    SortHeap(v, v + 9, CCompare());
    bool ok = true;
    for (int i = 0; i + 1 < 9; ++i)
        ok = ok && v[i].first <= v[i + 1].first;            // unsigned order: 0x80000000 and 0xFFFFFFF0 last
    ok = ok && v[0].first == 1 && v[8].first == 0xFFFFFFF0u && v[7].first == 0x80000000u;
    for (int i = 0; i < 9; ++i) {
        ok = ok && v[i].second.Kind() == 0xEE4;
        int src = (int)v[i].second.Index - 1000;           // the payload travelled with its key
        ok = ok && src >= 0 && src < 9 && keys[src] == v[i].first;
    }
    PairULongParamDesc one[1];
    one[0].first = 5;
    SortHeap(one, one + 1, CCompare());
    ok = ok && one[0].first == 5;
    if (!ok) { std::printf("0076b650_TEST FAIL\n"); return 1; }
    std::printf("0076b650_TEST PASS\n");
    return 0;
}