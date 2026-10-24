// Behaviour test for 006fcb30 (std::__insertion_sort of keyed event pairs through __linear_insert).
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

static void __linear_insert(SKeyedEvents* first, SKeyedEvents* last, SKeyedEvents val, CKeyPairCompareLess comp)   // SGI stand-in for 0x006FC3E0
{
    if (comp(val, *first)) {
        for (SKeyedEvents* p = last; p != first; --p)
            *p = *(p - 1);
        *first = val;
    }
    else {
        SKeyedEvents* next = last;
        --next;
        while (comp(val, *next)) {
            *last = *next;
            last = next;
            --next;
        }
        *last = val;
    }
}
static void __insertion_sort(SKeyedEvents* first, SKeyedEvents* last, CKeyPairCompareLess comp)
{
    if (first == last)
        return;
    for (SKeyedEvents* i = first + 1; i != last; ++i)
        __linear_insert(first, i, SKeyedEvents(*i), comp);
}

int main()
{
    const unsigned long keys[7] = { 5, 3, 8, 1, 9, 2, 7 };
    SKeyedEvents* v = (SKeyedEvents*)std::malloc(7 * sizeof(SKeyedEvents));
    for (int i = 0; i < 7; ++i)
        MakeKeyed(v + i, keys[i], (int)(keys[i] % 4));
    CKeyPairCompareLess comp;
    __insertion_sort(v, v + 7, comp);
    int ok = 1;
    for (int i = 0; i < 7; ++i)
        if ((i && v[i - 1].first > v[i].first) || !Consistent(v[i]))
            ok = 0;
    __insertion_sort(v, v, comp);
    if (!ok) {
        std::printf("006fcb30_TEST FAIL\n");
        return 1;
    }
    std::printf("006fcb30_TEST PASS\n");
    return 0;
}