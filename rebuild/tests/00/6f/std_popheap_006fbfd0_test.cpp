// Behaviour test for 006fbfd0 (std::__pop_heap of keyed event pairs: the top goes to *result and the rest is re-heaped).
#include <cstdio>
#include <cstdlib>
#include <new>

static int gLiveBlocks = 0;                     // CSmallVector blocks currently allocated

namespace NAnimationEvents {
class CEvent {
public:
    unsigned long Index : 30;
    unsigned long ExternallyImported : 1;
    unsigned long StopEvent : 1;
    float Time;
};
}

template<class T, int N> class CSmallVector {
public:
    T* Start;
    int Size;
    CSmallVector() : Start(0), Size(0) {}
    void Copy(const CSmallVector& other)        // stand-in for 0x006FB5C0: a deep copy
    {
        Clear();
        if (other.Size) {
            Start = new T[other.Size];
            ++gLiveBlocks;
            for (int i = 0; i < other.Size; ++i)
                Start[i] = other.Start[i];
        }
        Size = other.Size;
    }
    CSmallVector(const CSmallVector& other) : Start(0), Size(0) { Copy(other); }
    CSmallVector& operator=(const CSmallVector& other)
    {
        if (&other != this)
            Copy(other);
        return *this;
    }
    void Clear()
    {
        if (Start)
            --gLiveBlocks;
        delete[] Start;
        Start = 0;
        Size = 0;
    }
    ~CSmallVector() { Clear(); }
};

template<class K, class V> struct pair {
    K first;
    V second;
};

typedef CSmallVector<NAnimationEvents::CEvent, 32> SEvents;
typedef pair<unsigned long, SEvents> SKeyedEvents;

struct CKeyPairCompareLess {
    char m_tag;
    bool operator()(const SKeyedEvents& a, const SKeyedEvents& b) const { return a.first < b.first; }
};

static void MakeKeyed(SKeyedEvents* entry, unsigned long key, int events)
{
    new (entry) SKeyedEvents;
    entry->first = key;
    if (events) {
        entry->second.Start = new NAnimationEvents::CEvent[events];
        ++gLiveBlocks;
        for (int i = 0; i < events; ++i) {
            entry->second.Start[i].Index = key * 10 + i;
            entry->second.Start[i].Time = (float)key;
        }
        entry->second.Size = events;
    }
}

static int Consistent(const SKeyedEvents& e)    // the events still belong to their key
{
    for (int i = 0; i < e.second.Size; ++i)
        if (e.second.Start[i].Index != e.first * 10 + i)
            return 0;
    return 1;
}

// SGI helpers (stand-ins for the retail out-of-line bodies)
static void __push_heap(SKeyedEvents* first, long holeIndex, long topIndex, SKeyedEvents value, CKeyPairCompareLess comp)
{
    long parent = (holeIndex - 1) / 2;
    while (holeIndex > topIndex && comp(*(first + parent), value)) {
        *(first + holeIndex) = *(first + parent);
        holeIndex = parent;
        parent = (holeIndex - 1) / 2;
    }
    *(first + holeIndex) = value;
}
static void _Adjust_heap(SKeyedEvents* first, long holeIndex, long len, SKeyedEvents value, CKeyPairCompareLess comp)   // 0x006FBD60
{
    long topIndex = holeIndex;
    long secondChild = 2 * holeIndex + 2;
    while (secondChild < len) {
        if (comp(*(first + secondChild), *(first + (secondChild - 1))))
            secondChild--;
        *(first + holeIndex) = *(first + secondChild);
        holeIndex = secondChild;
        secondChild = 2 * (secondChild + 1);
    }
    if (secondChild == len) {
        *(first + holeIndex) = *(first + (secondChild - 1));
        holeIndex = secondChild - 1;
    }
    __push_heap(first, holeIndex, topIndex, value, comp);
}
static int IsHeap(const SKeyedEvents* first, long len)
{
    for (long i = 1; i < len; ++i)
        if (first[(i - 1) / 2].first < first[i].first)
            return 0;
    return 1;
}

static void __pop_heap(SKeyedEvents* first, SKeyedEvents* last, SKeyedEvents* result, SKeyedEvents value, CKeyPairCompareLess comp, long*)
{
    *result = *first;
    _Adjust_heap(first, long(0), long(last - first), value, comp);
}

int main()
{
    const unsigned long keys[6] = { 9, 7, 8, 3, 5, 6 };   // already a max-heap
    SKeyedEvents* v = (SKeyedEvents*)std::malloc(6 * sizeof(SKeyedEvents));
    for (int i = 0; i < 6; ++i)
        MakeKeyed(v + i, keys[i], 2);
    CKeyPairCompareLess comp;
    int ok = IsHeap(v, 6);
    SKeyedEvents last(v[5]);
    __pop_heap(v, v + 5, v + 5, last, comp, (long*)0);
    if (v[5].first != 9 || !IsHeap(v, 5) || v[0].first != 8 || !Consistent(v[5]) || !Consistent(v[0]))
        ok = 0;
    if (!ok) {
        std::printf("006fbfd0_TEST FAIL\n");
        return 1;
    }
    std::printf("006fbfd0_TEST PASS\n");
    return 0;
}