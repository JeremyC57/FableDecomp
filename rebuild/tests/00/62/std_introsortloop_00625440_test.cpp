// Behaviour test for 00625440: std::__introsort_loop over test quests with CTestQuestNameCompare (display name, or the
// quest name when it is empty, compared case-insensitively): median-of-three partition until 16 or fewer remain,
// partial_sort when the depth limit runs out; the final insertion sort then yields a sorted range.
#include <cstdio>
#include <cstring>

struct Q { const char* quest; const char* display; int id; };

static int ci_cmp(const char* a, const char* b)
{
    for (;; ++a, ++b) {
        int x = (*a >= 'A' && *a <= 'Z') ? *a + 32 : *a;
        int y = (*b >= 'A' && *b <= 'Z') ? *b + 32 : *b;
        if (x != y || !x) return x - y;
    }
}
static const char* name_of(const Q& q) { return q.display[0] ? q.display : q.quest; }
static bool comp(const Q& a, const Q& b) { return ci_cmp(name_of(a), name_of(b)) < 0; }

static const Q& median(const Q& a, const Q& b, const Q& c)
{
    if (comp(a, b))
        if (comp(b, c)) return b; else if (comp(a, c)) return c; else return a;
    else if (comp(a, c)) return a;
    else if (comp(b, c)) return c;
    else return b;
}
static Q* unguarded_partition(Q* first, Q* last, Q pivot)
{
    for (;;) {
        while (comp(*first, pivot)) ++first;
        --last;
        while (comp(pivot, *last)) --last;
        if (!(first < last)) return first;
        Q t = *first; *first = *last; *last = t;
        ++first;
    }
}
static void partial_sort_all(Q* first, Q* last)    // partial_sort(first, last, last): a full sort
{
    for (Q* i = first; i != last; ++i)
        for (Q* j = i + 1; j != last; ++j)
            if (comp(*j, *i)) { Q t = *i; *i = *j; *j = t; }
}
static int g_partial = 0;
static void introsort_loop(Q* first, Q* last, int depth_limit)
{
    while (last - first > 16) {
        if (depth_limit == 0) { ++g_partial; partial_sort_all(first, last); return; }
        --depth_limit;
        Q* cut = unguarded_partition(first, last, Q(median(*first, *(first + (last - first) / 2), *(last - 1))));
        introsort_loop(cut, last, depth_limit);
        last = cut;
    }
}
static void insertion_sort(Q* first, Q* last)
{
    for (Q* i = first + 1; i < last; ++i) {
        Q v = *i; Q* j = i;
        while (j != first && comp(v, *(j - 1))) { *j = *(j - 1); --j; }
        *j = v;
    }
}

static char g_names[60][8];
static bool run(int depth)
{
    Q q[60];
    for (int i = 0; i < 60; ++i) {
        int k = (i * 37 + 11) % 60;
        std::sprintf(g_names[i], "%c%c%02d", (k % 2) ? 'Q' : 'q', 'a' + k % 26, k);
        q[i].quest = g_names[i];
        q[i].display = (i % 3) ? "" : g_names[i];       // a third carry a display name equal to the quest name
        q[i].id = i;
    }
    introsort_loop(q, q + 60, depth);
    insertion_sort(q, q + 60);
    for (int i = 1; i < 60; ++i)
        if (comp(q[i], q[i - 1])) return false;
    return true;
}

int main()
{
    bool ok = run(2 * 5) && g_partial == 0;              // normal depth: partitions only
    ok = ok && run(0) && g_partial == 1;                 // depth exhausted: partial_sort the whole range
    Q a = { "Beta", "", 0 }, b = { "zzz", "alpha", 1 };
    ok = ok && comp(b, a) && !comp(a, b);                // display name wins when set; case-insensitive
    if (!ok) { std::printf("00625440_TEST FAIL\n"); return 1; }
    std::printf("00625440_TEST PASS\n");
    return 0;
}