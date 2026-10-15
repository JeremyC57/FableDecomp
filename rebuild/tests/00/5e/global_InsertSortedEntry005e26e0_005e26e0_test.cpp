// Behaviour test for 0x005E26E0 (insertion-sort step over (priority, def) pairs): repeated steps sort by priority and keep the reference counts balanced.
#include <cstdio>
struct Def { int refs; };
struct DefPtr {
    Def* Object;
    DefPtr(Def* d) : Object(d) { if (Object) ++Object->refs; }
    DefPtr(const DefPtr& o) : Object(o.Object) { if (Object) ++Object->refs; }
    DefPtr& operator=(const DefPtr& o) { if (Object != o.Object) { if (Object) --Object->refs; Object = o.Object; if (Object) ++Object->refs; } return *this; }
    ~DefPtr() { if (Object) --Object->refs; }
};
struct Entry { int first; DefPtr second; Entry(int p, Def* d) : first(p), second(d) {} };
struct Compare { bool operator()(const Entry& a, const Entry& b) const { return a.first < b.first; } };
static void CopyBackward(Entry* first, Entry* last, Entry* dest) { while (last != first) { --last; --dest; dest->first = last->first; dest->second = last->second; } }
static void UnguardedLinearInsert(Entry* last, Entry value, Compare pred)
{
    Entry* next = last; --next;
    while (pred(value, *next)) { last->first = next->first; last->second = next->second; last = next; --next; }
    last->first = value.first; last->second = value.second;
}
static void InsertSortedEntry(Entry* first, Entry* next, Entry value, Compare pred)
{
    if (pred(value, *first)) { CopyBackward(first, next, next + 1); first->first = value.first; first->second = value.second; }
    else UnguardedLinearInsert(next, value, pred);
}
int main()
{
    Def d[5] = { { 0 }, { 0 }, { 0 }, { 0 }, { 0 } };
    int ok = 1;
    {
        Entry e[5] = { Entry(4, &d[0]), Entry(2, &d[1]), Entry(9, &d[2]), Entry(1, &d[3]), Entry(5, &d[4]) };
        Compare c;
        for (int i = 1; i < 5; ++i) InsertSortedEntry(e, e + i, e[i], c);
        for (int i = 0; i + 1 < 5; ++i) if (e[i].first > e[i + 1].first) ok = 0;
        if (e[0].second.Object != &d[3] || e[4].second.Object != &d[2]) ok = 0;
        for (int i = 0; i < 5; ++i) if (d[i].refs != 1) ok = 0;
    }
    for (int i = 0; i < 5; ++i) if (d[i].refs != 0) ok = 0;
    if (!ok) { std::printf("005e26e0_TEST FAIL\n"); return 1; }
    std::printf("005e26e0_TEST PASS\n");
    return 0;
}