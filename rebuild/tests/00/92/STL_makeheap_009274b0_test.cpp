// Behaviour test for 0x009274B0 __make_heap: turns any range into a max-heap under the comparator; ranges below 2 are left alone.
#include <cstdio>

struct Item { int key; };

static int g_live = 0;                       // live CIntelligentPointer copies (by-value arguments must be destroyed)
class IPtr {                                 // CIntelligentPointer<const CThing> model
public:
    const Item* item;
    IPtr() : item(0) { ++g_live; }
    IPtr(const Item* p) : item(p) { ++g_live; }
    IPtr(const IPtr& o) : item(o.item) { ++g_live; }
    ~IPtr() { --g_live; }
    IPtr& operator=(const IPtr& o) { item = o.item; return *this; }
};
typedef bool (*IPtrCompare)(IPtr, IPtr);
static bool KeyLess(IPtr a, IPtr b) { return a.item->key < b.item->key; }

struct random_access_iterator_tag { };

template <class It, class Dist>
It CopyBackward(It first, It last, It result, const random_access_iterator_tag&, Dist*)
{
    for (Dist n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}

template <class T> void Swap(T& a, T& b) { T tmp = a; a = b; b = tmp; }

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

template <class It, class T, class Cmp>
void LinearInsert(It first, It last, T val, Cmp comp)
{
    if (comp(val, *first)) {
        CopyBackward(first, last, last + 1, random_access_iterator_tag(), (int*)0);
        *first = val;
    }
    else
        UnguardedLinearInsert(last, val, comp);
}

template <class It, class Cmp>
void InsertionSort(It first, It last, Cmp comp)
{
    if (first == last) return;
    for (It i = first + 1; i != last; ++i)
        LinearInsert(first, i, IPtr(*i), comp);
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
    int len = last - first;
    int parent = (len - 2) / 2;
    for (;;) {
        AdjustHeap(first, parent, len, IPtr(*(first + parent)), comp);
        if (parent == 0) return;
        parent--;
    }
}

template <class It, class Cmp>
void SortHeap(It first, It last, Cmp comp)
{
    while (last - first > 1) {
        IPtr val(*(last - 1));
        *(last - 1) = *first;
        AdjustHeap(first, 0, int((last - 1) - first), val, comp);
        --last;
    }
}

template <class It, class Cmp>
void PartialSort(It first, It middle, It last, Cmp comp)
{
    MakeHeap(first, middle, comp);
    for (It i = middle; i < last; ++i)
        if (comp(*i, *first)) {
            IPtr val(*i);
            *i = *first;
            AdjustHeap(first, 0, int(middle - first), val, comp);
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
        Swap(*first, *last);
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
        cut = UnguardedPartition(first, last, IPtr(Median(*first, *(first + (last - first) / 2), *(last - 1), comp)), comp);
        IntrosortLoop(cut, last, depth_limit, comp);
        last = cut;
    }
}

static Item g_items[64];
static void FillItems(int n, int seed)
{
    unsigned int s = (unsigned int)seed;
    for (int i = 0; i < n; ++i) {
        s = s * 1103515245u + 12345u;
        g_items[i].key = (int)((s >> 16) % 100u);
    }
}
static bool SortedByKey(IPtr* p, int n)
{
    for (int i = 1; i < n; ++i)
        if (p[i].item->key < p[i - 1].item->key) return false;
    return true;
}
static bool IsHeap(IPtr* p, int n)
{
    for (int i = 1; i < n; ++i)
        if (p[(i - 1) / 2].item->key < p[i].item->key) return false;
    return true;
}
static int KeySum(IPtr* p, int n)
{
    int s = 0;
    for (int i = 0; i < n; ++i) s += p[i].item->key;
    return s;
}

int main()
{
    int ok = 1;
    {
    for (int n = 0; n <= 20; ++n) {
        FillItems(n, n + 7);
        IPtr arr[20];
        for (int i = 0; i < n; ++i) arr[i] = IPtr(&g_items[i]);
        int sum = KeySum(arr, n);
        MakeHeap(arr, arr + n, (IPtrCompare)KeyLess);
        if (!IsHeap(arr, n) || KeySum(arr, n) != sum) ok = 0;
    }
    }
    if (g_live != 0) ok = 0;
    if (!ok) { std::printf("009274b0_TEST FAIL\n"); return 1; }
    std::printf("009274b0_TEST PASS\n");
    return 0;
}