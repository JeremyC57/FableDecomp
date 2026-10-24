// Behaviour test for 006fd330 (the scalar deleting destructor of NAnimationEvents::CManager: the extra-events map vector (each CSmallVector), then the symbol map, then the block).
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

static int gSymbolMapDtors = 0;
static int gDeletes = 0;
struct __false_type { __false_type() {} };
template<class T> inline void _Destroy(T* pointer) { pointer->~T(); }
template<class ForwardIterator> void __destroy_aux(ForwardIterator first, ForwardIterator last, const __false_type&)
{
    for (; first != last; ++first)
        _Destroy(&*first);
}
template<class ForwardIterator> inline void _Destroy(ForwardIterator first, ForwardIterator last)
{
    __destroy_aux(first, last, __false_type());
}
template<class T> class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~vector()
    {
        _Destroy(_M_start, _M_finish);
        if (_M_start)
            std::free(_M_start);
    }
};
template<class K, class V> class CVectorMap {
public:
    vector<pair<K, V> > Map;
    char Comp;
    bool Dirty;
};
class CSymbolMap {
public:
    unsigned char _body[0x24];
    ~CSymbolMap() { ++gSymbolMapDtors; }       // stand-in for 0x006FD040
};
class CManager {
public:
    virtual void GetEvents(unsigned long anim, void* events) const { (void)anim; (void)events; }
    const void* Bank;
    CSymbolMap EventSymbols;
    CVectorMap<unsigned long, SEvents> AnimToExtraEventsMap;
    bool Dirty;
    bool UseEmbeddedEvents;
    static void operator delete(void* block) { ++gDeletes; ::operator delete(block); }
};
void DeleteManager(CManager* manager) { delete manager; }

int main()
{
    CManager* manager = new CManager;
    SKeyedEvents* block = (SKeyedEvents*)std::malloc(3 * sizeof(SKeyedEvents));
    MakeKeyed(block + 0, 1, 2);
    MakeKeyed(block + 1, 2, 1);
    MakeKeyed(block + 2, 3, 0);
    manager->AnimToExtraEventsMap.Map._M_start = block;
    manager->AnimToExtraEventsMap.Map._M_finish = block + 3;
    manager->AnimToExtraEventsMap.Map._M_end_of_storage = block + 3;
    int ok = gLiveBlocks == 2;
    DeleteManager(manager);
    if (gLiveBlocks != 0 || gSymbolMapDtors != 1 || gDeletes != 1)
        ok = 0;
    if (!ok) {
        std::printf("006fd330_TEST FAIL\n");
        return 1;
    }
    std::printf("006fd330_TEST PASS\n");
    return 0;
}