// Behaviour test for 0x007485f0 std::__find_if over CTCStealth's danger-thing pairs (nonconst pointer instance)
#include <cstdio>
class CThing { public: int id; };

static int g_baseCtors = 0, g_sets = 0;
class CBaseIntelligentPointer {                         // stand-in for the 0x00A01Bxx helpers
public:
    const void* PItem;
    CBaseIntelligentPointer() : PItem(0) { ++g_baseCtors; }
    virtual ~CBaseIntelligentPointer() {}
    const void* GetPItem() const { return PItem; }
    void SetPItem(const void* item) { ++g_sets; PItem = item; }
};

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer() {}
    CIntelligentPointer(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); }
    T* Get() const { return (T*)GetPItem(); }
};

namespace NStealth {
enum EDangerStatus { DANGER_STATUS_NONE = 0, DANGER_STATUS_SUSPICIOUS = 1, DANGER_STATUS_DANGER = 2 };
class CDangerStatus {
public:
    EDangerStatus DangerStatus;
    int           Timer;
};
}

template <class A, class B> struct pair {
    A first;
    B second;
    pair() {}
    pair(const pair& other) : first(other.first), second(other.second) {}
};

typedef pair<CIntelligentPointer<const CThing>, NStealth::CDangerStatus> TDangerPair;

static void Set(TDangerPair& e, const CThing* thing, int status, int timer)
{
    e.first.SetPItem(thing);
    e.second.DangerStatus = (NStealth::EDangerStatus)status;
    e.second.Timer = timer;
}

class CFindDangerThing {
public:
    const CThing* Thing;
    CFindDangerThing(const CThing& thing) : Thing(&thing) {}
    bool operator()(const TDangerPair& entry) const { return entry.first.Get() == Thing; }
};

struct random_access_iterator_tag {};

template <class RandomAccessIter, class Predicate>
RandomAccessIter __find_if(RandomAccessIter first, RandomAccessIter last, Predicate pred,
                           const random_access_iterator_tag&)
{
    int trip_count = (last - first) >> 2;

    for (; trip_count > 0; --trip_count) {
        if (pred(*first)) return first;
        ++first;

        if (pred(*first)) return first;
        ++first;

        if (pred(*first)) return first;
        ++first;

        if (pred(*first)) return first;
        ++first;
    }

    switch (last - first) {
    case 3:
        if (pred(*first)) return first;
        ++first;
    case 2:
        if (pred(*first)) return first;
        ++first;
    case 1:
        if (pred(*first)) return first;
    case 0:
    default:
        return last;
    }
}

int main()
{
    CThing things[12];
    TDangerPair v[11];
    for (int i = 0; i < 11; ++i) { things[i].id = i; Set(v[i], &things[i], i % 3, i * 10); }
    TDangerPair* b = v;
    int ok = 1;
    random_access_iterator_tag tag;
    // every position (inside the unrolled trips and in each switch tail case) is found
    for (int n = 0; n <= 11; ++n)
        for (int i = 0; i < 11; ++i) {
            TDangerPair* r = __find_if(b, b + n, CFindDangerThing(things[i]), tag);
            if (r != (i < n ? b + i : b + n)) ok = 0;
        }
    // a thing that is not listed returns last
    if (__find_if(b, b + 11, CFindDangerThing(things[11]), tag) != b + 11) ok = 0;
    // the first of two equal entries wins
    Set(v[7], &things[2], 0, 0);
    if (__find_if(b, b + 11, CFindDangerThing(things[2]), tag) != b + 2) ok = 0;
    if (__find_if(b + 3, b + 11, CFindDangerThing(things[2]), tag) != b + 7) ok = 0;
    if (!ok) { std::printf("007485f0_TEST FAIL\n"); return 1; }
    std::printf("007485f0_TEST PASS\n");
    return 0;
}