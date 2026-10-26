// Behaviour test for 0x006BA7B0 std::__linear_insert(pair<unsigned long, CCountedPointer<CFaction> >*, .., val, comp).
#include <cstdio>

static int g_deleted = 0;
static void DeleteObject(void*) { ++g_deleted; }

class CCPPointerInfo { public: unsigned int RefCount; void (*DeleteFunc)(void*); void* Data; };
class CFaction { public: int id; };

struct CCountedPointer {
    CFaction*       Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) { }
    CCountedPointer(const CCountedPointer& o) : Data(o.Data), Info(o.Info) { if (Info) ++Info->RefCount; }
    ~CCountedPointer() { if (Info && --Info->RefCount == 0) { Info->DeleteFunc(Info->Data); delete Info; } }
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        CCPPointerInfo* info = other.Info;
        CFaction* data = other.Data;
        if (Info != info) {
            if (Info) { --Info->RefCount; if (Info->RefCount == 0) { Info->DeleteFunc(Info->Data); delete Info; } }
            Data = data; Info = info;
            if (info) ++info->RefCount;
        }
        return *this;
    }
};

struct Entry {
    unsigned int    first;
    CCountedPointer second;
    Entry() : first(0) { }
    Entry(const Entry& o) : first(o.first), second(o.second) { }
    Entry& operator=(const Entry& o) { first = o.first; second = o.second; return *this; }
};

struct KeyLess { bool operator()(const Entry& a, const Entry& b) const { return a.first < b.first; } };

static Entry* CopyBackward(Entry* first, Entry* last, Entry* result)
{
    for (long n = (long)(last - first); n > 0; --n)
        *--result = *--last;
    return result;
}

template <class RandomAccessIter, class T, class Compare>
void UnguardedLinearInsert(RandomAccessIter last, T val, Compare comp)
{
    RandomAccessIter next = last;
    --next;
    while (comp(val, *next)) { *last = *next; last = next; --next; }
    *last = val;
}

template <class RandomAccessIter, class T, class Compare>
void LinearInsert(RandomAccessIter first, RandomAccessIter last, T val, Compare comp)
{
    if (comp(val, *first)) {
        CopyBackward(first, last, last + 1);
        *first = val;
    }
    else
        UnguardedLinearInsert(last, val, comp);
}

static CFaction factions[8];

static void Own(Entry& e, unsigned int key, int i)
{
    e.first = key;
    e.second.Data = &factions[i];
    e.second.Info = new CCPPointerInfo;
    e.second.Info->RefCount = 1; e.second.Info->DeleteFunc = DeleteObject; e.second.Info->Data = &factions[i];
}

int main()
{
    int ok = 1;
    {
        Entry v[5];
        Own(v[0], 3, 0); Own(v[1], 6, 1); Own(v[2], 8, 2); Own(v[3], 1, 3);   // v[3] is the next element to insert
        LinearInsert(v, v + 3, Entry(v[3]), KeyLess());                         // new minimum: shift everything up
        if (v[0].first != 1 || v[1].first != 3 || v[2].first != 6 || v[3].first != 8) ok = 0;
        if (v[0].second.Data != &factions[3] || v[3].second.Data != &factions[2]) ok = 0;
        for (int i = 0; ok && i < 4; ++i) if (v[i].second.Info->RefCount != 1) ok = 0;
        Own(v[4], 7, 4);
        LinearInsert(v, v + 4, Entry(v[4]), KeyLess());                         // middle: unguarded walk
        if (v[0].first != 1 || v[1].first != 3 || v[2].first != 6 || v[3].first != 7 || v[4].first != 8) ok = 0;
        for (int i = 0; ok && i < 5; ++i) if (v[i].second.Info->RefCount != 1) ok = 0;
        if (g_deleted != 0) ok = 0;
    }
    if (g_deleted != 5) ok = 0;
    if (!ok) { std::printf("006ba7b0_TEST FAIL\n"); return 1; }
    std::printf("006ba7b0_TEST PASS\n");
    return 0;
}