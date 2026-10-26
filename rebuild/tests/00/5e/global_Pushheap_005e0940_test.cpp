// behaviour test for 0x005E0940 _Push_heap<pair<EAnimComponent,CIVCountedPointer<CAnimComponentBase> >, CKeyPairCompareLess>
#include <cstdio>
static int g_deleted = 0;
class CAnimComponentBase { public:
    unsigned int IVRefCount;
    int id;
    void Delete() { ++g_deleted; }
    void Release() { if (--IVRefCount == 0) Delete(); }
};
struct CIVCountedPointer {
    CAnimComponentBase* Object;
    CIVCountedPointer() : Object(0) { }
    explicit CIVCountedPointer(CAnimComponentBase* p) : Object(p) { if (p) ++p->IVRefCount; }
    CIVCountedPointer(const CIVCountedPointer& o) : Object(o.Object) { if (Object) ++Object->IVRefCount; }
    ~CIVCountedPointer() { if (Object) Object->Release(); }
    CIVCountedPointer& operator=(const CIVCountedPointer& other)
    {
        CAnimComponentBase* old = Object;
        if (old != other.Object) {
            if (old) { old->Release(); Object = 0; }
            Object = other.Object;
            if (Object) ++Object->IVRefCount;
        }
        return *this;
    }
};
struct PairAnimComponent { int first; CIVCountedPointer second; };
struct CKeyPairCompareLess {
    bool operator()(const PairAnimComponent& a, const PairAnimComponent& b) const { return a.first < b.first; }
};
static void _Push_heap(PairAnimComponent* first, long hole, long top, PairAnimComponent value, CKeyPairCompareLess comp)
{
    long parent = (hole - 1) / 2;
    while (hole > top && comp(first[parent], value)) {
        first[hole] = first[parent];
        hole = parent;
        parent = (hole - 1) / 2;
    }
    first[hole] = value;
}
int main()
{
    static const int keys[6] = { 4, 1, 6, 2, 9, 3 };
    CAnimComponentBase comps[6];
    for (int i = 0; i < 6; ++i) { comps[i].IVRefCount = 0; comps[i].id = i; }
    {
        PairAnimComponent heap[6];
        CKeyPairCompareLess c;
        for (int i = 0; i < 6; ++i) {
            PairAnimComponent v; v.first = keys[i]; v.second = CIVCountedPointer(&comps[i]);
            _Push_heap(heap, i, 0, v, c);
            for (int k = 1; k <= i; ++k)
                if (c(heap[(k - 1) / 2], heap[k])) { std::printf("005e0940_TEST FAIL\n"); return 1; }
        }
        if (heap[0].first != 9 || heap[0].second.Object != &comps[4]) { std::printf("005e0940_TEST FAIL\n"); return 1; }
        // every component is referenced exactly once by the heap, none released early
        for (int i = 0; i < 6; ++i)
            if (comps[i].IVRefCount != 1) { std::printf("005e0940_TEST FAIL\n"); return 1; }
        if (g_deleted != 0) { std::printf("005e0940_TEST FAIL\n"); return 1; }
    }
    if (g_deleted != 6) { std::printf("005e0940_TEST FAIL\n"); return 1; }
    std::printf("005e0940_TEST PASS\n");
    return 0;
}