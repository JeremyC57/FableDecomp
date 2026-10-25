// Behaviour test for 0x00979760 std::__introsort_loop (STLport) over CThing* with a 16-byte by-value comparator.
#include <cstdio>

struct CThing { float score; };
struct C2DVector { float X, Y; };
struct TComp {                                 // 16 bytes like the retail comparator; ranks by descending score
    C2DVector Pos, Dir;
    bool operator()(CThing* a, CThing* b) const { return a->score * Dir.X > b->score * Dir.X; }
};

static int g_partialSorts = 0;

static CThing* const& Median(CThing* const& a, CThing* const& b, CThing* const& c, TComp comp)
{
    if (comp(a, b)) { if (comp(b, c)) return b; else if (comp(a, c)) return c; else return a; }
    else if (comp(a, c)) return a;
    else if (comp(b, c)) return c;
    else return b;
}
static CThing** Partition(CThing** first, CThing** last, CThing* pivot, TComp comp)
{
    for (;;) {
        while (comp(*first, pivot)) ++first;
        --last;
        while (comp(pivot, *last)) --last;
        if (!(first < last)) return first;
        CThing* t = *first; *first = *last; *last = t;
        ++first;
    }
}
static void PartialSort(CThing** first, CThing** last, TComp comp)   // stand-in: selection sort of the whole range
{
    ++g_partialSorts;
    for (CThing** i = first; i != last; ++i)
        for (CThing** j = i + 1; j != last; ++j)
            if (comp(*j, *i)) { CThing* t = *i; *i = *j; *j = t; }
}
static void IntrosortLoop(CThing** first, CThing** last, CThing**, int depth, TComp comp)
{
    while (last - first > 16) {
        if (depth == 0) { PartialSort(first, last, comp); return; }
        --depth;
        CThing** cut = Partition(first, last, Median(*first, *(first + (last - first) / 2), *(last - 1), comp), comp);
        IntrosortLoop(cut, last, (CThing**)0, depth, comp);
        last = cut;
    }
}
static void InsertionSort(CThing** first, CThing** last, TComp comp)
{
    for (CThing** i = first + 1; i < last; ++i) {
        CThing* v = *i; CThing** j = i;
        while (j != first && comp(v, *(j - 1))) { *j = *(j - 1); --j; }
        *j = v;
    }
}

static bool RunSort(int n, int depth)
{
    CThing things[100];
    CThing* ptrs[100];
    for (int i = 0; i < n; ++i) { things[i].score = (float)((i * 37) % 101); ptrs[i] = &things[i]; }
    TComp comp; comp.Pos.X = 0; comp.Pos.Y = 0; comp.Dir.X = 1.0f; comp.Dir.Y = 0;
    IntrosortLoop(ptrs, ptrs + n, (CThing**)0, depth, comp);
    InsertionSort(ptrs, ptrs + n, comp);
    for (int i = 1; i < n; ++i) if (ptrs[i - 1]->score < ptrs[i]->score) return false;
    return true;
}

int main()
{
    if (!RunSort(100, 12) || !RunSort(10, 6) || g_partialSorts != 0) { std::printf("00979760_TEST FAIL\n"); return 1; }
    if (!RunSort(100, 0) || g_partialSorts != 1) { std::printf("00979760_TEST FAIL\n"); return 1; }
    std::printf("00979760_TEST PASS\n");
    return 0;
}