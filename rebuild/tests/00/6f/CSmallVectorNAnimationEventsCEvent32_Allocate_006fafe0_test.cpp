// Behaviour test for 006fafe0 (CSmallVector<CEvent,32>::Allocate: drop the old block, allocate n events and fill each with the value).
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

template<class T> class CEventArray {
public:
    T* Start;
    int Size;
    void Clear()
    {
        delete[] Start;
        Start = 0;
        Size = 0;
    }
    void Allocate(unsigned long n, const T& value)
    {
        Clear();
        Start = new T[n];
        Size = n;
        T* end = Start + n;
        for (T* p = Start; p != end; ++p)
            *p = value;
    }
};

int main()
{
    CEventArray<NAnimationEvents::CEvent> events;
    events.Start = 0;
    events.Size = 0;
    NAnimationEvents::CEvent value;
    value.Index = 0x3ffffffe;
    value.ExternallyImported = 1;
    value.StopEvent = 0;
    value.Time = 2.5f;
    events.Allocate(4, value);
    int ok = events.Size == 4 && events.Start != 0;
    for (int i = 0; ok && i < 4; ++i)
        if (events.Start[i].Index != 0x3ffffffe || events.Start[i].ExternallyImported != 1 || events.Start[i].StopEvent != 0 ||
            events.Start[i].Time != 2.5f)
            ok = 0;
    value.Time = -1.0f;
    events.Allocate(2, value);
    if (events.Size != 2 || events.Start[1].Time != -1.0f)
        ok = 0;
    events.Clear();
    if (events.Start != 0 || events.Size != 0)
        ok = 0;
    if (!ok) {
        std::printf("006fafe0_TEST FAIL\n");
        return 1;
    }
    std::printf("006fafe0_TEST PASS\n");
    return 0;
}