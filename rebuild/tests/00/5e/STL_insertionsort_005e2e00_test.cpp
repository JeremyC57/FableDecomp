// Behaviour test for 0x005E2E00 std::__insertion_sort over (priority, CDefPointer<const CContextSensitiveItemDef>) pairs.
#include <cstdio>

struct CIVCountedPointeeBase { unsigned long IVRefCount; int id; };
struct CDefPointer {
    CIVCountedPointeeBase* Object;
    explicit CDefPointer(CIVCountedPointeeBase* p) : Object(p) { if (Object) ++Object->IVRefCount; }
    CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) ++Object->IVRefCount; }
    ~CDefPointer() { if (Object) --Object->IVRefCount; }
    CDefPointer& operator=(const CDefPointer& o)
    {
        if (Object != o.Object) { if (Object) --Object->IVRefCount; Object = o.Object; if (Object) ++Object->IVRefCount; }
        return *this;
    }
};
struct SPriorityEntry {
    long first; CDefPointer second;
    SPriorityEntry(long p, CIVCountedPointeeBase* d) : first(p), second(d) { }
    SPriorityEntry(const SPriorityEntry& o) : first(o.first), second(o.second) { }
};
struct CContextSensitivePriorityCompare {
    void* m_context;
    bool operator()(const SPriorityEntry& a, const SPriorityEntry& b) const { return a.first < b.first; }
};

static int g_calls = 0;
static void __linear_insert(SPriorityEntry* first, SPriorityEntry* last, SPriorityEntry value, CContextSensitivePriorityCompare comp)
{
    ++g_calls;
    if (comp(value, *first)) {
        for (SPriorityEntry* p = last; p != first; --p) *p = *(p - 1);
        *first = value;
    } else {
        SPriorityEntry* next = last - 1;
        while (comp(value, *next)) { *last = *next; last = next; --next; }
        *last = value;
    }
}
static void __insertion_sort(SPriorityEntry* first, SPriorityEntry* last, CContextSensitivePriorityCompare comp)
{
    if (first == last)
        return;
    for (SPriorityEntry* i = first + 1; i != last; ++i)
        __linear_insert(first, i, *i, comp);
}

int main()
{
    CIVCountedPointeeBase defs[4] = { {0, 0}, {0, 1}, {0, 2}, {0, 3} };
    {
        SPriorityEntry a[4] = { SPriorityEntry(30, &defs[3]), SPriorityEntry(10, &defs[1]), SPriorityEntry(0, &defs[0]), SPriorityEntry(20, &defs[2]) };
        CContextSensitivePriorityCompare comp = { 0 };
        __insertion_sort(a, a + 4, comp);
        for (int k = 0; k < 4; ++k)
            if (a[k].first != 10 * k || a[k].second.Object->id != k || defs[k].IVRefCount != 1) { std::printf("005e2e00_TEST FAIL\n"); return 1; }
        if (g_calls != 3) { std::printf("005e2e00_TEST FAIL\n"); return 1; }
    }
    for (int k = 0; k < 4; ++k)
        if (defs[k].IVRefCount != 0) { std::printf("005e2e00_TEST FAIL\n"); return 1; }   // the by-value copies were all released
    std::printf("005e2e00_TEST PASS\n");
    return 0;
}