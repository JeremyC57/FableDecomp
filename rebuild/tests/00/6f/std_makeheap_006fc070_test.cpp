// Behaviour test for 006fc070 (std::__make_heap of keyed event pairs: SGI bottom-up heapify).
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

static void __make_heap(SKeyedEvents* first, SKeyedEvents* last, CKeyPairCompareLess comp, SKeyedEvents*, long*)
{
    if (last - first < 2)
        return;
    long len = last - first;
    long parent = (len - 2) / 2;
    for (;;) {
        _Adjust_heap(first, parent, len, SKeyedEvents(*(first + parent)), comp);
        if (parent == 0)
            return;
        parent--;
    }
}

int main()
{
    const unsigned long keys[8] = { 3, 1, 4, 1, 5, 9, 2, 6 };
    SKeyedEvents* v = (SKeyedEvents*)std::malloc(8 * sizeof(SKeyedEvents));
    for (int i = 0; i < 8; ++i)
        MakeKeyed(v + i, keys[i], 1);
    CKeyPairCompareLess comp;
    __make_heap(v, v + 8, comp, (SKeyedEvents*)0, (long*)0);
    int ok = IsHeap(v, 8) && v[0].first == 9;
    for (int i = 0; i < 8; ++i)
        if (!Consistent(v[i]))
            ok = 0;
    __make_heap(v, v + 1, comp, (SKeyedEvents*)0, (long*)0);
    if (v[0].first != 9)
        ok = 0;
    if (!ok) {
        std::printf("006fc070_TEST FAIL\n");
        return 1;
    }
    std::printf("006fc070_TEST PASS\n");
    return 0;
}