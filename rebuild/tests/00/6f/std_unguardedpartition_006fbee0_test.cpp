// Behaviour test for 006fbee0 (std::__unguarded_partition of keyed event pairs around a pivot key).
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

template<class T> inline void __iter_swap(T* a, T* b)
{
    T tmp = *a;
    *a = *b;
    *b = tmp;
}
static SKeyedEvents* __unguarded_partition(SKeyedEvents* first, SKeyedEvents* last, SKeyedEvents pivot, CKeyPairCompareLess comp)
{
    for (;;) {
        while (comp(*first, pivot))
            ++first;
        --last;
        while (comp(pivot, *last))
            --last;
        if (!(first < last))
            return first;
        __iter_swap(first, last);
        ++first;
    }
}

int main()
{
    const unsigned long keys[7] = { 7, 2, 9, 4, 5, 1, 8 };
    SKeyedEvents* v = (SKeyedEvents*)std::malloc(7 * sizeof(SKeyedEvents));
    for (int i = 0; i < 7; ++i)
        MakeKeyed(v + i, keys[i], (int)(keys[i] % 3));
    SKeyedEvents pivot;
    MakeKeyed(&pivot, 5, 1);
    CKeyPairCompareLess comp;
    SKeyedEvents* cut = __unguarded_partition(v, v + 7, pivot, comp);
    int ok = cut > v && cut < v + 7;
    for (SKeyedEvents* p = v; p < cut; ++p)
        if (p->first > 5)
            ok = 0;
    for (SKeyedEvents* p = cut; p < v + 7; ++p)
        if (p->first < 5)
            ok = 0;
    unsigned long sum = 0;
    for (int i = 0; i < 7; ++i) {
        sum += v[i].first;
        if (!Consistent(v[i]))
            ok = 0;
    }
    if (sum != 36)
        ok = 0;
    if (!ok) {
        std::printf("006fbee0_TEST FAIL\n");
        return 1;
    }
    std::printf("006fbee0_TEST PASS\n");
    return 0;
}