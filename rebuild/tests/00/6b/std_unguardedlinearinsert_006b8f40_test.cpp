// Behaviour test for 0x006B8F40 std::__unguarded_linear_insert(pair<unsigned long, CCountedPointer<CFaction> >*, pair, CKeyPairCompareLess).
#include <cstdio>

static int g_deleted = 0;
static void DeleteObject(void*) { ++g_deleted; }

struct CCPPointerInfo {
    unsigned int RefCount;
    void (*DeleteFunc)(void*);
    void* Data;
};

struct CFaction { int id; };

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
            if (Info) {
                --Info->RefCount;
                if (Info->RefCount == 0) { Info->DeleteFunc(Info->Data); delete Info; }
            }
            Data = data;
            Info = info;
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
};

struct KeyLess { bool operator()(const Entry& a, const Entry& b) const { return a.first < b.first; } };

template <class RandomAccessIter, class T, class Compare>
void UnguardedLinearInsert(RandomAccessIter last, T val, Compare comp)
{
    RandomAccessIter next = last;
    --next;
    while (comp(val, *next)) {
        *last = *next;
        last = next;
        --next;
    }
    *last = val;
}

static CFaction factions[4];

static void Make(Entry& e, unsigned int key, CFaction* f)
{
    e.first = key;
    CCountedPointer p;
    p.Data = f;
    p.Info = new CCPPointerInfo;
    p.Info->RefCount = 1; p.Info->DeleteFunc = DeleteObject; p.Info->Data = f;
    e.second = p;                                       // e owns one reference; p's dies with p
    --p.Info->RefCount;                                 // drop the temporary's own count without deleting
    p.Info = 0;
}

int main()
{
    {
        Entry v[4];
        Make(v[0], 2, &factions[0]); Make(v[1], 5, &factions[1]); Make(v[2], 9, &factions[2]); Make(v[3], 4, &factions[3]);
        UnguardedLinearInsert(v + 3, v[3], KeyLess());
        int ok = v[0].first == 2 && v[1].first == 4 && v[2].first == 5 && v[3].first == 9 &&
                 v[1].second.Data == &factions[3] && v[2].second.Data == &factions[1] && v[3].second.Data == &factions[2];
        for (int i = 0; ok && i < 4; ++i)
            if (v[i].second.Info->RefCount != 1) ok = 0;
        if (g_deleted != 0) ok = 0;
        // already in place: nothing moves
        UnguardedLinearInsert(v + 3, v[3], KeyLess());
        if (v[3].first != 9 || v[3].second.Info->RefCount != 1 || g_deleted != 0) ok = 0;
        if (!ok) { std::printf("006b8f40_TEST FAIL\n"); return 1; }
    }
    if (g_deleted != 4) { std::printf("006b8f40_TEST FAIL\n"); return 1; }
    std::printf("006b8f40_TEST PASS\n");
    return 0;
}