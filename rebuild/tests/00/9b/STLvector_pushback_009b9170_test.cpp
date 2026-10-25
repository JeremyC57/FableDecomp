#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>

// Host model of pair<unsigned long, CDefinitionManager::CDefClassInfo>: a name, the class's alloc function and a
// deep-copied CArray<unsigned long> of global indices (malloc + memmove copy, as the STLport vector copy).
static int g_live = 0;                                    // live index arrays (copies must be deep and freed)
struct CIndexArray {
    unsigned int* Start; unsigned int* Finish; unsigned int* End;
    CIndexArray() : Start(0), Finish(0), End(0) { }
    CIndexArray(const CIndexArray& x) : Start(0), Finish(0), End(0)
    {
        unsigned int n = (unsigned int)(x.Finish - x.Start);
        if (n != 0) { Start = (unsigned int*)std::malloc(n * sizeof(unsigned int)); ++g_live; }
        Finish = Start; End = Start + n;
        if (x.Finish != x.Start) { std::memmove(Start, x.Start, (x.Finish - x.Start) * sizeof(unsigned int)); Finish = Start + n; }
    }
    CIndexArray& operator=(const CIndexArray& x)
    {
        if (this != &x) { CIndexArray t(x); unsigned int* s = Start; Start = t.Start; t.Start = s; unsigned int* f = Finish; Finish = t.Finish; t.Finish = f; unsigned int* e = End; End = t.End; t.End = e; }
        return *this;
    }
    ~CIndexArray() { if (Start) { std::free(Start); --g_live; } }
    void push(unsigned int v)
    {
        unsigned int n = (unsigned int)(Finish - Start);
        unsigned int* p = (unsigned int*)std::malloc((n + 1) * sizeof(unsigned int));
        if (n) std::memcpy(p, Start, n * sizeof(unsigned int));
        if (Start) std::free(Start); else ++g_live;
        Start = p; Finish = p + n + 1; End = Finish; p[n] = v;
    }
};
struct CDefClassInfo {
    const char* Name;
    int AllocFunc;
    CIndexArray GlobalIndices;
};
template <class T1, class T2> struct pair { T1 first; T2 second; };
typedef pair<unsigned int, CDefClassInfo> TEntry;
struct CKeyPairCompareLess {
    bool operator()(const TEntry& a, const TEntry& b) const { return a.first < b.first; }
};

// vector<pair<unsigned long, CDefClassInfo> >::push_back: construct in place while there is room, else grow
// (_M_insert_overflow doubles the storage and copies the old elements across).
struct CInfoVector {
    TEntry* Start; TEntry* Finish; TEntry* End;
    CInfoVector() : Start(0), Finish(0), End(0) { }
    ~CInfoVector() { for (TEntry* p = Start; p != Finish; ++p) p->~TEntry(); std::free(Start); }
    void insert_overflow(TEntry* pos, const TEntry& x)
    {
        unsigned int old = (unsigned int)(Finish - Start);
        unsigned int len = old ? 2 * old : 1;
        TEntry* s = (TEntry*)std::malloc(len * sizeof(TEntry));
        TEntry* d = s;
        for (TEntry* p = Start; p != pos; ++p, ++d) new (d) TEntry(*p);
        new (d) TEntry(x); ++d;
        for (TEntry* p = Start; p != Finish; ++p) p->~TEntry();
        std::free(Start);
        Start = s; Finish = d; End = s + len;
    }
    void push_back(const TEntry& x)
    {
        if (Finish != End) {
            if (Finish) new (Finish) TEntry(x);
            ++Finish;
        }
        else
            insert_overflow(Finish, x);
    }
};

// ---- copy of the STLport 4.x sort/heap algorithms the retail TU instantiates (plain templates for the host) ----
namespace model {
template <class T> inline void iter_swap_(T* a, T* b) { T t = *a; *a = *b; *b = t; }
template <class T, class C> inline const T& median_(const T& a, const T& b, const T& c, C comp)
{
    if (comp(a, b))
        if (comp(b, c)) return b; else if (comp(a, c)) return c; else return a;
    else if (comp(a, c)) return a;
    else if (comp(b, c)) return c;
    else return b;
}
template <class T, class C> T* unguarded_partition(T* first, T* last, T pivot, C comp)
{
    for (;;) {
        while (comp(*first, pivot)) ++first;
        --last;
        while (comp(pivot, *last)) --last;
        if (!(first < last)) return first;
        iter_swap_(first, last);
        ++first;
    }
}
template <class T, class C> void unguarded_linear_insert(T* last, T val, C comp)
{
    T* next = last; --next;
    while (comp(val, *next)) { *last = *next; last = next; --next; }
    *last = val;
}
template <class T, class C> void linear_insert(T* first, T* last, T val, C comp)
{
    if (comp(val, *first)) {
        for (T* p = last; p != first; --p) *p = *(p - 1);
        *first = val;
    } else
        unguarded_linear_insert(last, val, comp);
}
template <class T, class C> void insertion_sort(T* first, T* last, C comp)
{
    if (first == last) return;
    for (T* i = first + 1; i != last; ++i) linear_insert(first, i, *i, comp);
}
template <class T, class C> void unguarded_insertion_sort(T* first, T* last, C comp)
{
    for (T* i = first; i != last; ++i) unguarded_linear_insert(i, *i, comp);
}
template <class T, class C> void final_insertion_sort(T* first, T* last, C comp)
{
    if (last - first > 16) {
        insertion_sort(first, first + 16, comp);
        unguarded_insertion_sort(first + 16, last, comp);
    } else
        insertion_sort(first, last, comp);
}
template <class T, class C> void push_heap_(T* first, int hole, int top, T val, C comp)
{
    int parent = (hole - 1) / 2;
    while (hole > top && comp(*(first + parent), val)) { *(first + hole) = *(first + parent); hole = parent; parent = (hole - 1) / 2; }
    *(first + hole) = val;
}
template <class T, class C> void adjust_heap(T* first, int hole, int len, T val, C comp)
{
    int top = hole;
    int second = 2 * hole + 2;
    while (second < len) {
        if (comp(*(first + second), *(first + (second - 1)))) second--;
        *(first + hole) = *(first + second);
        hole = second;
        second = 2 * (second + 1);
    }
    if (second == len) { *(first + hole) = *(first + (second - 1)); hole = second - 1; }
    push_heap_(first, hole, top, val, comp);
}
template <class T, class C> void make_heap_(T* first, T* last, C comp)
{
    if (last - first < 2) return;
    int len = (int)(last - first);
    int parent = (len - 2) / 2;
    for (;;) {
        adjust_heap(first, parent, len, T(*(first + parent)), comp);
        if (parent == 0) return;
        parent--;
    }
}
template <class T, class C> void sort_heap_(T* first, T* last, C comp)
{
    while (last - first > 1) {
        --last;
        T val = *last;
        *last = *first;
        adjust_heap(first, 0, (int)(last - first), val, comp);
    }
}
template <class T, class C> void partial_sort_(T* first, T* middle, T* last, C comp)
{
    make_heap_(first, middle, comp);
    for (T* i = middle; i < last; ++i)
        if (comp(*i, *first)) { T val = *i; *i = *first; adjust_heap(first, 0, (int)(middle - first), val, comp); }
    sort_heap_(first, middle, comp);
}
inline int lg_(int n) { int k; for (k = 0; n != 1; n >>= 1) ++k; return k; }
template <class T, class C> void introsort_loop(T* first, T* last, int depth, C comp)
{
    while (last - first > 16) {
        if (depth == 0) { partial_sort_(first, last, last, comp); return; }
        --depth;
        T* cut = unguarded_partition(first, last, T(median_(*first, *(first + (last - first) / 2), *(last - 1), comp)), comp);
        introsort_loop(cut, last, depth, comp);
        last = cut;
    }
}
template <class T, class C> void sort_(T* first, T* last, C comp)
{
    if (first != last) {
        introsort_loop(first, last, lg_((int)(last - first)) * 2, comp);
        final_insertion_sort(first, last, comp);
    }
}
template <class T, class C> bool is_sorted_(const T* first, const T* last, C comp)
{
    for (const T* p = first; p + 1 < last; ++p) if (comp(*(p + 1), *p)) return false;
    return true;
}
}
// ---- end of the algorithm copy ----

int main()
{
    {
        CInfoVector vec;
        for (unsigned int i = 0; i < 9; ++i) {
            TEntry e;
            e.first = i * 3;
            e.second.Name = "info";
            e.second.AllocFunc = (int)i;
            e.second.GlobalIndices.push(i + 1000);
            vec.push_back(e);
        }
        if (vec.Finish - vec.Start != 9) { std::printf("009b9170_TEST FAIL\n"); return 1; }
        for (unsigned int i = 0; i < 9; ++i) {
            if (vec.Start[i].first != i * 3 || vec.Start[i].second.AllocFunc != (int)i) { std::printf("009b9170_TEST FAIL\n"); return 1; }
            if (vec.Start[i].second.GlobalIndices.Start[0] != i + 1000) { std::printf("009b9170_TEST FAIL\n"); return 1; }
        }
        if (g_live != 9) { std::printf("009b9170_TEST FAIL\n"); return 1; }                                        // each element owns its own copy
    }
    if (g_live != 0) { std::printf("009b9170_TEST FAIL\n"); return 1; }
    std::printf("009b9170_TEST PASS\n");
    return 0;
}