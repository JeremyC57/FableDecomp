// Behaviour test for 0x006BA100 std::__unguarded_partition(pair<unsigned long, CCountedPointer<CFaction> >*, .., pivot, comp).
#include <cstdio>

static int g_deleted = 0;
static void DeleteObject(void*) { ++g_deleted; }

struct CCPPointerInfo { unsigned int RefCount; void (*DeleteFunc)(void*); void* Data; };
struct CFaction { int id; };

struct CCountedPointer {
    CFaction*       Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) { }
    CCountedPointer(const CCountedPointer& o) : Data(o.Data), Info(o.Info) { if (Info) ++Info->RefCount; }
    ~CCountedPointer() { if (Info && --Info->RefCount == 0) { Info->DeleteFunc(Info->Data); delete Info; } }
};

struct Entry {
    unsigned int    first;
    CCountedPointer second;
    Entry() : first(0) { }
    Entry(const Entry& o) : first(o.first), second(o.second) { }
};

struct KeyLess { bool operator()(const Entry& a, const Entry& b) const { return a.first < b.first; } };

static void IterSwap(Entry* a, Entry* b)            // stand-in for 0x006B8E70: swap the words (counts unchanged)
{
    Entry t; t.first = a->first; t.second.Data = a->second.Data; t.second.Info = a->second.Info;
    a->first = b->first; a->second.Data = b->second.Data; a->second.Info = b->second.Info;
    b->first = t.first; b->second.Data = t.second.Data; b->second.Info = t.second.Info;
    t.second.Info = 0;
}

template <class RandomAccessIter, class T, class Compare>
RandomAccessIter UnguardedPartition(RandomAccessIter first, RandomAccessIter last, T pivot, Compare comp)
{
    for (;;) {
        while (comp(*first, pivot))
            ++first;
        --last;
        while (comp(pivot, *last))
            --last;
        if (!(first < last))
            return first;
        IterSwap(first, last);
        ++first;
    }
}

int main()
{
    unsigned int keys[7] = { 7, 1, 9, 5, 3, 8, 2 };
    Entry v[7];
    for (int i = 0; i < 7; ++i) v[i].first = keys[i];
    Entry pivot; pivot.first = 5;
    pivot.second.Info = new CCPPointerInfo; pivot.second.Info->RefCount = 1; pivot.second.Info->DeleteFunc = DeleteObject; pivot.second.Info->Data = 0;
    Entry* cut = UnguardedPartition(v, v + 7, pivot, KeyLess());
    int ok = cut > v && cut < v + 7;
    for (Entry* p = v; ok && p < cut; ++p) if (p->first > 5) ok = 0;
    for (Entry* p = cut; ok && p < v + 7; ++p) if (p->first < 5) ok = 0;
    if (pivot.second.Info->RefCount != 1 || g_deleted != 0) ok = 0;   // the by-value copy was released
    if (!ok) { std::printf("006ba100_TEST FAIL\n"); return 1; }
    std::printf("006ba100_TEST PASS\n");
    return 0;
}