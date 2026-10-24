// Behaviour test for 0x00711B80 std::__stable_sort_aux: temporary buffer (filled with *first), adaptive sort, buffer freed;
// the in-place path when no buffer is available.
#include <cstdio>
#include <cstdlib>

struct SNode { SNode* next; SNode* prev; long Priority; int Id; };
struct It { SNode* node; };                                 // list<CAnimSet>::const_iterator stand-in
struct Less { bool operator()(const It& a, const It& b) const { return a.node->Priority < b.node->Priority; } };

static SNode g_nodes[64];
static void Fill(It* v, int n, const long* pr)
{
    for (int i = 0; i < n; ++i) { g_nodes[i].Priority = pr[i]; g_nodes[i].Id = i; v[i].node = &g_nodes[i]; }
}
static bool SortedStable(const It* v, int n)
{
    for (int i = 1; i < n; ++i) {
        if (v[i].node->Priority < v[i - 1].node->Priority) return false;
        if (v[i].node->Priority == v[i - 1].node->Priority && v[i].node->Id < v[i - 1].node->Id) return false;
    }
    return true;
}
static It* CopyBackward(It* first, It* last, It* result) { for (int n = (int)(last - first); n > 0; --n) *--result = *--last; return result; }
static It* Merge(It* f1, It* l1, It* f2, It* l2, It* r, Less c)
{
    while (f1 != l1 && f2 != l2) { if (c(*f2, *f1)) *r++ = *f2++; else *r++ = *f1++; }
    while (f1 != l1) *r++ = *f1++;
    while (f2 != l2) *r++ = *f2++;
    return r;
}
static void InsertionSort(It* first, It* last, Less c)
{
    if (first == last) return;
    for (It* i = first + 1; i != last; ++i) {
        It val = *i;
        if (c(val, *first)) { CopyBackward(first, i, i + 1); *first = val; }
        else { It* l = i; It* nx = i - 1; while (c(val, *nx)) { *l = *nx; l = nx; --nx; } *l = val; }
    }
}
static void ChunkInsertionSort(It* first, It* last, int chunk, Less c)
{
    while (last - first >= chunk) { InsertionSort(first, first + chunk, c); first += chunk; }
    InsertionSort(first, last, c);
}
static void MergeSortLoop(It* first, It* last, It* result, int step, Less c)
{
    int two = 2 * step;
    while (last - first >= two) { result = Merge(first, first + step, first + step, first + two, result, c); first += two; }
    int rest = (int)(last - first);
    step = rest < step ? rest : step;
    Merge(first, first + step, first + step, last, result, c);
}
static void MergeSortWithBuffer(It* first, It* last, It* buffer, Less c)
{
    int len = (int)(last - first);
    It* bl = buffer + len;
    int step = 7;
    ChunkInsertionSort(first, last, step, c);
    while (step < len) { MergeSortLoop(first, last, buffer, step, c); step *= 2; MergeSortLoop(buffer, bl, first, step, c); step *= 2; }
}
static It* MergeBackward(It* f1, It* l1, It* f2, It* l2, It* r, Less c)
{
    if (f1 == l1) return CopyBackward(f2, l2, r);
    if (f2 == l2) return CopyBackward(f1, l1, r);
    --l1; --l2;
    for (;;) {
        if (c(*l2, *l1)) { *--r = *l1; if (f1 == l1) return CopyBackward(f2, ++l2, r); --l1; }
        else { *--r = *l2; if (f2 == l2) return CopyBackward(f1, ++l1, r); --l2; }
    }
}
static void MergeAdaptiveSimple(It* first, It* middle, It* last, It* buffer, int bufsize, Less c)
{
    // the buffered merge for runs that fit (the retail 0x00711340 handles the rest by rotation)
    int len1 = (int)(middle - first);
    It* be = buffer; for (It* p = first; p != middle; ++p) *be++ = *p;
    Merge(buffer, be, middle, last, first, c);
    (void)len1; (void)bufsize;
}
static void StableSortAdaptive(It* first, It* last, It* buffer, int bufsize, Less c)
{
    int len = (int)((last - first + 1) / 2);
    It* middle = first + len;
    if (len > bufsize) { StableSortAdaptive(first, middle, buffer, bufsize, c); StableSortAdaptive(middle, last, buffer, bufsize, c); }
    else { MergeSortWithBuffer(first, middle, buffer, c); MergeSortWithBuffer(middle, last, buffer, c); }
    MergeAdaptiveSimple(first, middle, last, buffer, bufsize, c);
}

static bool g_noBuffer = false;
static int g_frees = 0;
static void StableSortAux(It* first, It* last, Less c)
{
    int len = (int)(last - first);
    It* buffer = 0;
    int blen = len;
    if (!g_noBuffer && blen > 0) buffer = (It*)std::malloc(blen * sizeof(It));
    if (buffer) for (int i = 0; i < blen; ++i) buffer[i] = *first;
    if (buffer == 0) InsertionSort(first, last, c);           // __inplace_stable_sort for short ranges
    else StableSortAdaptive(first, last, buffer, blen, c);
    std::free(buffer); ++g_frees;
}
int main()
{
    It v[30]; long pr[30];
    for (int i = 0; i < 30; ++i) pr[i] = (i * 13) % 5;
    Fill(v, 30, pr);
    StableSortAux(v, v + 30, Less());
    bool ok = SortedStable(v, 30) && g_frees == 1;
    It w[12]; long pw[12]; for (int i = 0; i < 12; ++i) pw[i] = (i * 5) % 3;
    Fill(w, 12, pw); g_noBuffer = true;
    StableSortAux(w, w + 12, Less());
    ok = ok && SortedStable(w, 12) && g_frees == 2;
    if (!ok) { std::printf("00711b80_TEST FAIL\n"); return 1; }
    std::printf("00711b80_TEST PASS\n");
    return 0;
}