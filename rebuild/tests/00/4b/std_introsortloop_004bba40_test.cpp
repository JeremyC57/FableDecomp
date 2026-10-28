// behaviour test for 0x004BBA40 std::__introsort_loop (STLport 4.6 over (quest name, info) entries ordered by key)
#include <cstdio>
#include <cstring>
#include <algorithm>
struct Entry { const char* Key; int Info; };
struct Less { bool operator()(const Entry& a, const Entry& b) const { return std::strcmp(a.Key, b.Key) < 0; } };
static const Entry& median(const Entry& a, const Entry& b, const Entry& c, Less comp)
{
    if (comp(a, b)) { if (comp(b, c)) return b; else if (comp(a, c)) return c; else return a; }
    else if (comp(a, c)) return a; else if (comp(b, c)) return c; else return b;
}
static Entry* partition(Entry* first, Entry* last, Entry pivot, Less comp)
{
    for (;;) {
        while (comp(*first, pivot)) ++first;
        --last;
        while (comp(pivot, *last)) --last;
        if (!(first < last)) return first;
        std::swap(*first, *last);
        ++first;
    }
}
static int g_partial = 0;
static void introsort_loop(Entry* first, Entry* last, int depth, Less comp)
{
    while (last - first > 16) {
        if (depth == 0) { ++g_partial; std::partial_sort(first, last, last, comp); return; }
        --depth;
        Entry* cut = partition(first, last, Entry(median(*first, *(first + (last - first) / 2), *(last - 1), comp)), comp);
        introsort_loop(cut, last, depth, comp);
        last = cut;
    }
}
int main()
{
    static char names[64][8];
    Entry e[64];
    for (int i = 0; i < 64; ++i) { int k = (i * 37 + 11) % 64; std::sprintf(names[i], "Q%03d", k); e[i].Key = names[i]; e[i].Info = k; }
    introsort_loop(e, e + 64, 2 * 6, Less());
    std::sort(e, e + 64, Less());                       // the final insertion sort's job
    bool ok = true;
    for (int i = 0; i < 64; ++i) ok = ok && e[i].Info == i;
    Entry f[40];
    for (int i = 0; i < 40; ++i) { f[i].Key = names[i]; f[i].Info = 0; }
    introsort_loop(f, f + 40, 0, Less());               // depth exhausted: partial (heap) sort of everything
    for (int i = 1; i < 40; ++i) ok = ok && std::strcmp(f[i - 1].Key, f[i].Key) <= 0;
    ok = ok && g_partial >= 1;
    std::printf(ok ? "004bba40_TEST PASS\n" : "004bba40_TEST FAIL\n");
    return ok ? 0 : 1;
}