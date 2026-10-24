// Behaviour test for 006fbb30 (std::__uninitialized_fill_n of keyed event pairs: n deep copies of one pair).
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

struct __false_type { __false_type() {} };
template<class T1, class T2> inline void _Construct(T1* pointer, const T2& value) { new (pointer) T1(value); }
static SKeyedEvents* __uninitialized_fill_n(SKeyedEvents* first, unsigned int n, const SKeyedEvents& x, const __false_type&)
{
    SKeyedEvents* cur = first;
    for (; n > 0; --n, ++cur)
        _Construct(&*cur, x);
    return cur;
}

int main()
{
    SKeyedEvents source;
    MakeKeyed(&source, 9, 3);
    SKeyedEvents* raw = (SKeyedEvents*)std::malloc(3 * sizeof(SKeyedEvents));
    SKeyedEvents* end = __uninitialized_fill_n(raw, 3u, source, __false_type());
    int ok = end == raw + 3 && gLiveBlocks == 4;
    for (int i = 0; i < 3; ++i)
        if (raw[i].first != 9 || raw[i].second.Size != 3 || raw[i].second.Start == source.second.Start || !Consistent(raw[i]))
            ok = 0;
    if (__uninitialized_fill_n(raw, 0u, source, __false_type()) != raw)
        ok = 0;
    if (!ok) {
        std::printf("006fbb30_TEST FAIL\n");
        return 1;
    }
    std::printf("006fbb30_TEST PASS\n");
    return 0;
}