// Behaviour test for 0x0091FA00 __median<CThing*, CThingCompare_Nearest>: returns a reference to the middle element of the three by distance.
#include <cstdio>
#include <cstring>

class C3DVector { public: float X, Y, Z; };
class CThing {
public:
    float Z;                                    // the position is (0, 0, Z) in this test
};
static int g_compares = 0;
// CThingCompare_Nearest: the retail operator() (0x0091F740) returns the nearer of the two things (a null one loses),
// which is truthy for any non-null pair; the test exercises the algorithm with a strict stand-in that returns `a` only
// when it is strictly nearer to Pos than `b` (and null otherwise), so the sort order is well defined.
class CThingCompare_Nearest {
public:
    C3DVector Pos;
    template <class T> T* operator()(T* a, T* b) const
    {
        ++g_compares;
        float da = a->Z - Pos.Z, db = b->Z - Pos.Z;
        return da * da < db * db ? a : 0;
    }
};
typedef CThingCompare_Nearest ThingCompare;
static bool LowerFirst(CThing*, CThing*) { return false; }

template <class It, class T, class Cmp>
void UnguardedLinearInsert(It last, T val, Cmp comp)
{
    It next = last;
    --next;
    while (comp(val, *next)) {
        *last = *next;
        last = next;
        --next;
    }
    *last = val;
}

template <class It, class Cmp>
void InsertionSort(It first, It last, Cmp comp)
{
    if (first == last) return;
    for (It i = first + 1; i != last; ++i) {
        CThing* val = *i;
        if (comp(val, *first)) {
            int n = (int)((char*)i - (char*)first);
            if (n > 0) std::memmove((char*)(i + 1) - n, first, n);
            *first = val;
        }
        else
            UnguardedLinearInsert(i, val, comp);
    }
}

template <class It, class Dist, class T, class Cmp>
void PushHeap(It first, Dist hole, Dist top, T val, Cmp comp)
{
    Dist parent = (hole - 1) / 2;
    while (hole > top && comp(*(first + parent), val)) {
        *(first + hole) = *(first + parent);
        hole = parent;
        parent = (hole - 1) / 2;
    }
    *(first + hole) = val;
}

template <class It, class Dist, class T, class Cmp>
void AdjustHeap(It first, Dist hole, Dist len, T val, Cmp comp)
{
    Dist top = hole;
    Dist second = 2 * hole + 2;
    while (second < len) {
        if (comp(*(first + second), *(first + (second - 1))))
            second--;
        *(first + hole) = *(first + second);
        hole = second;
        second = 2 * (second + 1);
    }
    if (second == len) {
        *(first + hole) = *(first + (second - 1));
        hole = second - 1;
    }
    PushHeap(first, hole, top, val, comp);
}

template <class It, class Cmp>
void MakeHeap(It first, It last, Cmp comp)
{
    if (last - first < 2) return;
    int len = (int)(last - first);
    int parent = (len - 2) / 2;
    for (;;) {
        AdjustHeap(first, parent, len, *(first + parent), comp);
        if (parent == 0) return;
        parent--;
    }
}

template <class It, class Cmp>
void SortHeap(It first, It last, Cmp comp)
{
    while (last - first > 1) {
        CThing* val = *(last - 1);
        *(last - 1) = *first;
        AdjustHeap(first, 0, (int)((last - 1) - first), val, comp);
        --last;
    }
}

template <class It, class Cmp>
void PartialSort(It first, It middle, It last, Cmp comp)
{
    MakeHeap(first, middle, comp);
    for (It i = middle; i < last; ++i)
        if (comp(*i, *first)) {
            CThing* val = *i;
            *i = *first;
            AdjustHeap(first, 0, (int)(middle - first), val, comp);
        }
    SortHeap(first, middle, comp);
}

template <class T, class Cmp>
const T& Median(const T& a, const T& b, const T& c, Cmp comp)
{
    if (comp(a, b)) {
        if (comp(b, c)) return b;
        else if (comp(a, c)) return c;
        else return a;
    }
    else if (comp(a, c)) return a;
    else if (comp(b, c)) return c;
    else return b;
}

template <class It, class T, class Cmp>
It UnguardedPartition(It first, It last, T pivot, Cmp comp)
{
    for (;;) {
        while (comp(*first, pivot)) ++first;
        --last;
        while (comp(pivot, *last)) --last;
        if (!(first < last)) return first;
        T tmp = *first; *first = *last; *last = tmp;
        ++first;
    }
}

template <class It, class Cmp>
void IntrosortLoop(It first, It last, int depth_limit, Cmp comp)
{
    It cut;
    while (last - first > 16) {
        if (depth_limit == 0) {
            PartialSort(first, last, last, comp);
            return;
        }
        --depth_limit;
        cut = UnguardedPartition(first, last, Median(*first, *(first + (last - first) / 2), *(last - 1), comp), comp);
        IntrosortLoop(cut, last, depth_limit, comp);
        last = cut;
    }
}

static CThing g_things[64];
static CThing* g_ptrs[64];
static void Fill(int n, int seed)
{
    unsigned int s = (unsigned int)seed;
    for (int i = 0; i < n; ++i) {
        s = s * 1103515245u + 12345u;
        g_things[i].Z = (float)((s >> 16) % 50u) * 0.5f;
        g_ptrs[i] = &g_things[i];
    }
}
static bool Sorted(CThing** p, int n)          // nearest to Z = 0 first (all keys are >= 0)
{
    for (int i = 1; i < n; ++i)
        if (p[i]->Z < p[i - 1]->Z) return false;
    return true;
}
static ThingCompare Nearest()
{
    ThingCompare c;
    c.Pos.X = 0.0f; c.Pos.Y = 0.0f; c.Pos.Z = 0.0f;
    return c;
}
template <class Size> Size Lg(Size n)
{
    Size k;
    for (k = 0; n != 1; n >>= 1)
        ++k;
    return k;
}
template <class It, class Cmp> void FinalInsertionSort(It first, It last, Cmp comp)
{
    if (last - first > 16) {
        InsertionSort(first, first + 16, comp);
        for (It i = first + 16; i != last; ++i)
            UnguardedLinearInsert(i, *i, comp);
    }
    else
        InsertionSort(first, last, comp);
}
template <class It, class Cmp> void Sort(It first, It last, Cmp comp)
{
    if (first != last) {
        IntrosortLoop(first, last, Lg((int)(last - first)) * 2, comp);
        FinalInsertionSort(first, last, comp);
    }
}
static bool SamePointers(CThing** p, int n)        // a permutation of g_things[0..n)
{
    int seen[64];
    for (int i = 0; i < n; ++i) seen[i] = 0;
    for (int i = 0; i < n; ++i) {
        int k = (int)(p[i] - g_things);
        if (k < 0 || k >= n || seen[k]) return false;
        seen[k] = 1;
    }
    return true;
}

int main()
{
    int ok = 1;
    float zs[6][3] = { { 1, 2, 3 }, { 3, 2, 1 }, { 2, 1, 3 }, { 2, 3, 1 }, { 1, 3, 2 }, { 3, 1, 2 } };
    for (int k = 0; k < 6; ++k) {
        CThing t[3];
        CThing* p[3];
        for (int i = 0; i < 3; ++i) { t[i].Z = zs[k][i]; p[i] = &t[i]; }
        CThing* const& m = Median(p[0], p[1], p[2], Nearest());
        if (m->Z != 2.0f) ok = 0;
        if (&m != &p[0] && &m != &p[1] && &m != &p[2]) ok = 0;     // a reference into the arguments
    }
    (void)LowerFirst;
    if (!ok) { std::printf("0091fa00_TEST FAIL\n"); return 1; }
    std::printf("0091fa00_TEST PASS\n");
    return 0;
}