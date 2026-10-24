// Behaviour test for 006fb8a0 (std::__destroy_aux over the keyed event pairs: every CSmallVector block is released and zeroed).
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
template<class T> inline void _Destroy(T* pointer) { pointer->~T(); }
static void __destroy_aux(SKeyedEvents* first, SKeyedEvents* last, const __false_type&)
{
    for (; first != last; ++first)
        _Destroy(&*first);
}

int main()
{
    SKeyedEvents* block = (SKeyedEvents*)std::malloc(4 * sizeof(SKeyedEvents));
    MakeKeyed(block + 0, 1, 2);
    MakeKeyed(block + 1, 2, 0);
    MakeKeyed(block + 2, 3, 5);
    MakeKeyed(block + 3, 4, 1);
    int ok = gLiveBlocks == 3;
    __destroy_aux(block + 1, block + 4, __false_type());
    if (gLiveBlocks != 1 || block[2].second.Start != 0 || block[2].second.Size != 0 || block[0].second.Size != 2)
        ok = 0;
    __destroy_aux(block, block + 1, __false_type());
    if (gLiveBlocks != 0)
        ok = 0;
    std::free(block);
    if (!ok) {
        std::printf("006fb8a0_TEST FAIL\n");
        return 1;
    }
    std::printf("006fb8a0_TEST PASS\n");
    return 0;
}