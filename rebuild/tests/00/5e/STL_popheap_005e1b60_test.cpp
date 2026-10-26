// Behaviour test for 0x005E1B60 std::__pop_heap over pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase> >.
#include <cstdio>
#include <cstring>

enum EAnimComponent { ANIM_COMPONENT_A = 1, ANIM_COMPONENT_B, ANIM_COMPONENT_C, ANIM_COMPONENT_D, ANIM_COMPONENT_E };
class CAnimComponentBase { public:
    long IVRefCount;
    int  Deletes;
    CAnimComponentBase() : IVRefCount(0), Deletes(0) {}
    virtual ~CAnimComponentBase() {}
    virtual void Delete() { ++Deletes; }
};
template <class T> class CIVCountedPointer { public:
    T* Object;
    explicit CIVCountedPointer(T* o) : Object(o) { if (Object) ++Object->IVRefCount; }
    CIVCountedPointer(const CIVCountedPointer& other) : Object(other.Object) { if (Object) ++Object->IVRefCount; }
    void Release() { T* object = Object; if (object) { if (--object->IVRefCount == 0) object->Delete(); Object = 0; } }
    ~CIVCountedPointer() { Release(); }
    CIVCountedPointer& operator=(const CIVCountedPointer& other)
    {
        T* object = Object; T* incoming = other.Object;
        if (object != incoming) { Release(); Object = other.Object; if (Object) ++Object->IVRefCount; }
        return *this;
    }
};
namespace stlp {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& a, const _T2& b) : first(a), second(b) {}
    template <class _U1, class _U2> pair(const pair<_U1, _U2>& p) : first(p.first), second(p.second) {}
    pair(const pair& p) : first(p.first), second(p.second) {}
};
}
typedef stlp::pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase> > SPair;
struct TLess { bool operator()(const SPair& a, const SPair& b) const { return a.first < b.first; } };
// STLport __adjust_heap + __push_heap (the callee 0x005E1210), max-heap by key
static void PushHeap(SPair* first, int hole, int top, SPair val, TLess comp)
{
    int parent = (hole - 1) / 2;
    while (hole > top && comp(first[parent], val)) { first[hole] = first[parent]; hole = parent; parent = (hole - 1) / 2; }
    first[hole] = val;
}
static void AdjustHeap(SPair* first, int hole, int len, SPair val, TLess comp)
{
    int top = hole; int second = 2 * hole + 2;
    while (second < len) { if (comp(first[second], first[second - 1])) --second; first[hole] = first[second]; hole = second; second = 2 * (second + 1); }
    if (second == len) { first[hole] = first[second - 1]; hole = second - 1; }
    PushHeap(first, hole, top, val, comp);
}
static void PopHeap(SPair* first, SPair* last, SPair* result, SPair val, TLess comp, int*)
{
    *result = *first;
    AdjustHeap(first, 0, int(last - first), val, comp);
}

int main()
{
    CAnimComponentBase c[5];
    // a max-heap of keys 5,4,3,1,2 (each component referenced once by the heap)
    SPair h[5] = { SPair(ANIM_COMPONENT_E, CIVCountedPointer<CAnimComponentBase>(&c[4])),
                   SPair(ANIM_COMPONENT_D, CIVCountedPointer<CAnimComponentBase>(&c[3])),
                   SPair(ANIM_COMPONENT_C, CIVCountedPointer<CAnimComponentBase>(&c[2])),
                   SPair(ANIM_COMPONENT_A, CIVCountedPointer<CAnimComponentBase>(&c[0])),
                   SPair(ANIM_COMPONENT_B, CIVCountedPointer<CAnimComponentBase>(&c[1])) };
    for (int i = 0; i < 5; ++i) c[i].IVRefCount = 1;
    PopHeap(h, h + 4, h + 4, SPair(h[4]), TLess(), (int*)0);
    bool ok = h[1].first <= h[0].first && h[2].first <= h[0].first && h[3].first <= h[1].first;
    for (int i = 0; i < 5; ++i) if (c[i].Deletes != 0) ok = false;
    if (!(h[4].first == ANIM_COMPONENT_E && h[4].second.Object == &c[4] && h[0].first == ANIM_COMPONENT_D && ok && c[0].IVRefCount == 1 && c[4].IVRefCount == 1)) {
        std::printf("005e1b60_TEST FAIL\n");
        return 1;
    }
    std::printf("005e1b60_TEST PASS\n");
    return 0;
}