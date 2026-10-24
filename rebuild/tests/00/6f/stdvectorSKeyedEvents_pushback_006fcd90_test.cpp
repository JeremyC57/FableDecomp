// Behaviour test for 006fcd90 (STLport vector<pair<unsigned long, CSmallVector<CEvent,32> > >::push_back: copy-construct at finish or grow).
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

class CKeyedVector {
public:
    SKeyedEvents* _M_start;
    SKeyedEvents* _M_finish;
    SKeyedEvents* _M_end_of_storage;
    int Overflows;
    void _M_insert_overflow(SKeyedEvents* position, const SKeyedEvents& x, const __false_type&, unsigned int fill_len, bool atend)
    {   // the real grow (0x006FC6A0) reduced to the push_back case
        ++Overflows;
        if (!atend || fill_len != 1 || position != _M_finish)
            return;
        long size = (long)(_M_finish - _M_start);
        long len = size ? 2 * size : 1;
        SKeyedEvents* fresh = (SKeyedEvents*)std::malloc(len * sizeof(SKeyedEvents));
        for (long i = 0; i < size; ++i) {
            _Construct(fresh + i, _M_start[i]);
            _M_start[i].~SKeyedEvents();
        }
        _Construct(fresh + size, x);
        std::free(_M_start);
        _M_start = fresh;
        _M_finish = fresh + size + 1;
        _M_end_of_storage = fresh + len;
    }
    void push_back(const SKeyedEvents& x)
    {
        if (_M_finish != _M_end_of_storage) {
            _Construct(_M_finish, x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, __false_type(), 1UL, true);
    }
};

int main()
{
    CKeyedVector v;
    v._M_start = (SKeyedEvents*)std::malloc(sizeof(SKeyedEvents));
    v._M_finish = v._M_start;
    v._M_end_of_storage = v._M_start + 1;
    v.Overflows = 0;
    SKeyedEvents a, b;
    MakeKeyed(&a, 4, 2);
    MakeKeyed(&b, 6, 3);
    v.push_back(a);
    int ok = v.Overflows == 0 && v._M_finish == v._M_start + 1 && v._M_start[0].second.Start != a.second.Start;
    v.push_back(b);
    if (v.Overflows != 1 || v._M_finish - v._M_start != 2 || v._M_start[1].first != 6 || !Consistent(v._M_start[0]) ||
        !Consistent(v._M_start[1]) || gLiveBlocks != 4)
        ok = 0;
    if (!ok) {
        std::printf("006fcd90_TEST FAIL\n");
        return 1;
    }
    std::printf("006fcd90_TEST PASS\n");
    return 0;
}