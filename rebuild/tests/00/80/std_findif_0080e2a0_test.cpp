// Behaviour test for 0080e2a0: std::find_if over pair<CIntelligentPointer<CThing>, long> entries with CIsFirstElementEqualTo (the 4x-unrolled random-access __find_if)
#include <cstdio>

class CThing { public: int Id; };
class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer() : PData(0) { }
    virtual ~CBaseIntelligentPointer() { }
    void* GetPItem() const { return PData; }
    void SetPItem(void* item) { PData = item; }
    void* PData;
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer() { }
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};
template <class T1, class T2> struct Pair {
    T1 first;
    T2 second;
};
struct RandomAccessIteratorTag { RandomAccessIteratorTag() { } };
template <class RandomAccessIter, class Predicate>
RandomAccessIter FindIfImpl(RandomAccessIter first, RandomAccessIter last, Predicate pred, const RandomAccessIteratorTag&)
{
    int trip_count = (int)(last - first) >> 2;
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
template <class InputIter, class Predicate>
InputIter FindIf(InputIter first, InputIter last, Predicate pred)
{
    return FindIfImpl(first, last, pred, RandomAccessIteratorTag());
}
typedef Pair<CIntelligentPointer<CThing>, long> CThingEntry;
struct CIsFirstElementEqualTo {
    CIntelligentPointer<CThing> Thing;
    bool operator()(const CThingEntry& entry) const { return entry.first.GetPItem() == Thing.GetPItem(); }
};

int main()
{
    CThing things[7];
    CThingEntry entries[7];
    for (int i = 0; i < 7; ++i) { things[i].Id = i; entries[i].first.SetPItem(&things[i]); entries[i].second = 10 * i; }
    bool ok = true;
    for (int n = 0; n <= 7; ++n) {
        for (int k = 0; k < 7; ++k) {
            CIsFirstElementEqualTo pred;
            pred.Thing.SetPItem(&things[k]);
            CThingEntry* r = FindIf(entries, entries + n, pred);
            ok = ok && r == (k < n ? entries + k : entries + n);
        }
    }
    CThing other;
    CIsFirstElementEqualTo miss;
    miss.Thing.SetPItem(&other);
    ok = ok && FindIf(entries, entries + 7, miss) == entries + 7;
    if (!ok) { std::printf("0080e2a0_TEST FAIL\n"); return 1; }
    std::printf("0080e2a0_TEST PASS\n");
    return 0;
}