// Behaviour test for 0x009E6A70 std::__find_if over pair<CCharString, CSymbolInfo> with a group-name predicate
// (four-way unrolled scan plus the 3/2/1 tail; CCharString equality by block identity, then length + Compare).
#include <cstdio>

struct Data { const char* m_pData; unsigned long m_length; };
static long Compare(const char* a, const char* b)
{
    for (;;) {
        char ca = *a;
        if (ca == 0 && *b == 0) return 0;
        char cb = *b;
        if (ca < cb) return -1;
        if (ca > cb) return 1;
        ++a; ++b;
    }
}
struct Str {
    Data* PStringData;
    bool operator==(const Str& o) const
    {
        if (PStringData == o.PStringData) return true;
        if (PStringData && o.PStringData && PStringData->m_length == o.PStringData->m_length)
            return Compare(PStringData->m_pData, o.PStringData->m_pData) ? false : true;
        return false;
    }
};
struct Info { Str GroupName; Str EntryName; long Val; };
struct Entry { Str first; Info second; };
struct GroupEquals {
    Str GroupName;
    bool operator()(const Entry& e) const { return e.second.GroupName == GroupName; }
};

static Entry* FindIf(Entry* first, Entry* last, GroupEquals pred)
{
    int trip = (int)(last - first) >> 2;
    for (; trip > 0; --trip) {
        if (pred(*first)) return first; ++first;
        if (pred(*first)) return first; ++first;
        if (pred(*first)) return first; ++first;
        if (pred(*first)) return first; ++first;
    }
    switch (last - first) {
    case 3: if (pred(*first)) return first; ++first;
    case 2: if (pred(*first)) return first; ++first;
    case 1: if (pred(*first)) return first;
    case 0:
    default: return last;
    }
}

int main()
{
    int ok = 1;
    Data a = { "alpha", 5 }, b = { "beta", 4 }, b2 = { "beta", 4 }, g = { "gamma", 5 }, d = { "delta", 5 };
    Entry e[7];
    Data* groups[7] = { &a, 0, &g, &a, &d, &b, &g };
    for (int i = 0; i < 7; ++i) { e[i].first.PStringData = 0; e[i].second.GroupName.PStringData = groups[i];
        e[i].second.EntryName.PStringData = 0; e[i].second.Val = i; }
    GroupEquals p;
    p.GroupName.PStringData = &b2;                       // same text, different block, found in the tail
    if (FindIf(e, e + 7, p) != e + 5) ok = 0;
    p.GroupName.PStringData = &g;                        // same block, found in the unrolled loop
    if (FindIf(e, e + 7, p) != e + 2) ok = 0;
    p.GroupName.PStringData = &d;
    if (FindIf(e, e + 4, p) != e + 4) ok = 0;            // not in range -> last
    p.GroupName.PStringData = 0;                         // unassigned matches the unassigned entry only
    if (FindIf(e, e + 7, p) != e + 1) ok = 0;
    Data x = { "omega", 5 };
    p.GroupName.PStringData = &x;                        // equal length, different text
    if (FindIf(e, e + 7, p) != e + 7) ok = 0;
    if (FindIf(e, e, p) != e) ok = 0;
    if (!ok) { std::printf("009e6a70_TEST FAIL\n"); return 1; }
    std::printf("009e6a70_TEST PASS\n");
    return 0;
}