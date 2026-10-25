// Behaviour test for 00cbd5b0: CVectorMap<u64,CPersistInfo>::Sort -- the vector range is sorted by key and Dirty cleared
#include <cstdio>
struct CPersistInfo { unsigned __int64 UID; int Script; };
struct Pair { unsigned __int64 first; CPersistInfo second; };
struct Less { bool operator()(const Pair& a, const Pair& b) const { return a.first < b.first; } };
static int g_sorts = 0;
static void Sort(Pair* first, Pair* last, Less comp)
{
    ++g_sorts;
    for (Pair* i = first; i != last; ++i)
        for (Pair* j = i + 1; j != last; ++j)
            if (comp(*j, *i)) { Pair t = *i; *i = *j; *j = t; }
}
struct CVectorMap {
    Pair* m_start; Pair* m_finish; Pair* m_end; Less Compare; bool Dirty;
    void SortMap() { Sort(m_start, m_finish, Compare); Dirty = false; }
};
int main()
{
    Pair p[3]; p[0].first = 0x500000000ULL; p[1].first = 7; p[2].first = 0x100000000ULL;
    CVectorMap m; m.m_start = p; m.m_finish = p + 3; m.m_end = p + 3; m.Dirty = true;
    m.SortMap();
    int ok = g_sorts == 1 && !m.Dirty && p[0].first == 7 && p[1].first == 0x100000000ULL && p[2].first == 0x500000000ULL;
    std::printf(ok ? "00cbd5b0_TEST PASS\n" : "00cbd5b0_TEST FAIL\n");
    return ok ? 0 : 1;
}
