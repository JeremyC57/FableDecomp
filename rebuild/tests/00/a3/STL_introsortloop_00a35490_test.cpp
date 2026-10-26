// Behaviour test for 0x00A35490 std::__introsort_loop: partitions until every run is at most 16 entries with the runs ordered (the final insertion sort finishes them); a zero depth limit heap-sorts instead.
#include <cstdio>

static int g_strings = 0;      // live CCharString objects
static int g_released = 0;     // count blocks freed
class CCharString {
public:
    int Id;
    CCharString() : Id(0) { ++g_strings; }
    explicit CCharString(int id) : Id(id) { ++g_strings; }
    CCharString(const CCharString& o) : Id(o.Id) { ++g_strings; }
    ~CCharString() { --g_strings; }
    CCharString& operator=(const CCharString& o) { Id = o.Id; return *this; }
};
class CCPPointerInfo { public:
    int   RefCount;
    void (*DeleteFunc)(void* data);
    void* Data;
};
static void DeleteVolume(void*) { ++g_released; }
template <class T> class CCountedPointer { public:
    T* Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) { }
    CCountedPointer(const CCountedPointer& o) : Data(o.Data), Info(o.Info) { if (Info) ++Info->RefCount; }
    ~CCountedPointer()
    {
        if (Info && --Info->RefCount == 0) { Info->DeleteFunc(Info->Data); delete Info; }
        Data = 0; Info = 0;
    }
    void ShareData(T* data, CCPPointerInfo* info)
    {
        if (Info != info) {
            if (Info) { --Info->RefCount; if (Info->RefCount == 0) { Info->DeleteFunc(Info->Data); delete Info; } }
            Data = data; Info = info;
            if (Info) ++Info->RefCount;
        }
    }
    CCountedPointer& operator=(const CCountedPointer& o) { ShareData(o.Data, o.Info); return *this; }
};
class CContainmentVolume { public: int Tag; };
class C3DMeshVolume { public:
    CCharString Name;
    CCountedPointer<CContainmentVolume> ConvexVolume;
    C3DMeshVolume& operator=(const C3DMeshVolume& o) { Name = o.Name; ConvexVolume = o.ConvexVolume; return *this; }
};
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair() { }
    pair(const pair& p) : first(p.first), second(p.second) { }
};
typedef pair<unsigned int, C3DMeshVolume> TEntry;
struct CKeyPairCompareLess {
    bool operator()(const TEntry& a, const TEntry& b) const { return a.first < b.first; }
};
struct random_access_iterator_tag { random_access_iterator_tag() { } };

static CContainmentVolume g_vol[64];
static void Fill(TEntry* a, const unsigned int* keys, int n)
{
    for (int i = 0; i < n; ++i) {
        a[i].first = keys[i];
        a[i].second.Name.Id = 100 + i;
        g_vol[i].Tag = 100 + i;
        CCPPointerInfo* info = new CCPPointerInfo;
        info->RefCount = 1; info->DeleteFunc = DeleteVolume; info->Data = &g_vol[i];
        a[i].second.ConvexVolume.Data = &g_vol[i];
        a[i].second.ConvexVolume.Info = info;
    }
}
// every slot's name and pointer still describe the same original element, each block held exactly once per slot
static bool Consistent(const TEntry* a, int n)
{
    for (int i = 0; i < n; ++i) {
        if (a[i].second.ConvexVolume.Data == 0 || a[i].second.ConvexVolume.Data->Tag != a[i].second.Name.Id) return false;
        int refs = 0;
        for (int j = 0; j < n; ++j) if (a[j].second.ConvexVolume.Info == a[i].second.ConvexVolume.Info) ++refs;
        if (a[i].second.ConvexVolume.Info->RefCount != refs) return false;
    }
    return g_released == 0;
}
static bool Sorted(const TEntry* a, int n)
{
    for (int i = 1; i < n; ++i) if (a[i].first < a[i - 1].first) return false;
    return true;
}

template <class T> inline void swap(T& a, T& b) { T tmp = a; a = b; b = tmp; }
template <class RandomAccessIter, class OutputIter, class Distance>
OutputIter __copy(RandomAccessIter first, RandomAccessIter last, OutputIter result, const random_access_iterator_tag&, Distance*)
{
    for (Distance n = (Distance)(last - first); n > 0; --n) { *result = *first; ++first; ++result; }
    return result;
}
template <class BI1, class BI2, class Distance>
BI2 __copy_backward(BI1 first, BI1 last, BI2 result, const random_access_iterator_tag&, Distance*)
{
    for (Distance n = (Distance)(last - first); n > 0; --n) *--result = *--last;
    return result;
}
template <class RandomAccessIter, class T, class Compare>
void __unguarded_linear_insert(RandomAccessIter last, T val, Compare comp)
{
    RandomAccessIter next = last;
    --next;
    while (comp(val, *next)) { *last = *next; last = next; --next; }
    *last = val;
}
template <class RandomAccessIter, class T, class Compare>
void __unguarded_insertion_sort_aux(RandomAccessIter first, RandomAccessIter last, T*, Compare comp)
{
    for (RandomAccessIter i = first; i != last; ++i)
        __unguarded_linear_insert(i, T(*i), comp);
}
template <class RandomAccessIter, class T, class Compare>
void __linear_insert(RandomAccessIter first, RandomAccessIter last, T val, Compare comp)
{
    if (comp(val, *first)) {
        __copy_backward(first, last, last + 1, random_access_iterator_tag(), (int*)0);
        *first = val;
    }
    else
        __unguarded_linear_insert(last, val, comp);
}
template <class RandomAccessIter, class Compare>
void __insertion_sort(RandomAccessIter first, RandomAccessIter last, Compare comp)
{
    if (first == last) return;
    for (RandomAccessIter i = first + 1; i != last; ++i)
        __linear_insert(first, i, TEntry(*i), comp);
}
template <class RandomAccessIter, class T, class Compare>
RandomAccessIter __unguarded_partition(RandomAccessIter first, RandomAccessIter last, T pivot, Compare comp)
{
    for (;;) {
        while (comp(*first, pivot)) ++first;
        --last;
        while (comp(pivot, *last)) --last;
        if (!(first < last)) return first;
        swap(*first, *last);
        ++first;
    }
}
template <class RandomAccessIter, class T, class Compare>
void __push_heap(RandomAccessIter first, int holeIndex, int topIndex, T val, Compare comp)
{
    int parent = (holeIndex - 1) / 2;
    while (holeIndex > topIndex && comp(*(first + parent), val)) {
        *(first + holeIndex) = *(first + parent);
        holeIndex = parent;
        parent = (holeIndex - 1) / 2;
    }
    *(first + holeIndex) = val;
}
template <class RandomAccessIter, class T, class Compare>
void __adjust_heap(RandomAccessIter first, int holeIndex, int len, T val, Compare comp)
{
    int topIndex = holeIndex;
    int secondChild = 2 * holeIndex + 2;
    while (secondChild < len) {
        if (comp(*(first + secondChild), *(first + (secondChild - 1)))) secondChild--;
        *(first + holeIndex) = *(first + secondChild);
        holeIndex = secondChild;
        secondChild = 2 * (secondChild + 1);
    }
    if (secondChild == len) { *(first + holeIndex) = *(first + (secondChild - 1)); holeIndex = secondChild - 1; }
    __push_heap(first, holeIndex, topIndex, val, comp);
}
template <class RandomAccessIter, class Compare, class T, class Distance>
void __make_heap(RandomAccessIter first, RandomAccessIter last, Compare comp, T*, Distance*)
{
    if (last - first < 2) return;
    Distance len = (Distance)(last - first);
    Distance parent = (len - 2) / 2;
    for (;;) {
        __adjust_heap(first, parent, len, T(*(first + parent)), comp);
        if (parent == 0) return;
        parent--;
    }
}
template <class RandomAccessIter, class T, class Compare, class Distance>
void __pop_heap(RandomAccessIter first, RandomAccessIter last, RandomAccessIter result, T value, Compare comp, Distance*)
{
    *result = *first;
    __adjust_heap(first, 0, (int)(last - first), value, comp);
}
template <class RandomAccessIter, class Compare>
void sort_heap(RandomAccessIter first, RandomAccessIter last, Compare comp)
{
    while (last - first > 1) {
        --last;
        __pop_heap(first, last, last, TEntry(*last), comp, (int*)0);
    }
}
template <class RandomAccessIter, class T, class Compare>
void __partial_sort(RandomAccessIter first, RandomAccessIter middle, RandomAccessIter last, T*, Compare comp)
{
    __make_heap(first, middle, comp, (T*)0, (int*)0);
    for (RandomAccessIter i = middle; i < last; ++i)
        if (comp(*i, *first))
            __pop_heap(first, middle, i, T(*i), comp, (int*)0);
    sort_heap(first, middle, comp);
}
template <class T, class Compare>
inline const T& __median(const T& a, const T& b, const T& c, Compare comp)
{
    if (comp(a, b))
        if (comp(b, c)) return b;
        else if (comp(a, c)) return c;
        else return a;
    else if (comp(a, c)) return a;
    else if (comp(b, c)) return c;
    else return b;
}
template <class RandomAccessIter, class T, class Size, class Compare>
void __introsort_loop(RandomAccessIter first, RandomAccessIter last, T*, Size depth_limit, Compare comp)
{
    while (last - first > 16) {
        if (depth_limit == 0) { __partial_sort(first, last, last, (T*)0, comp); return; }
        --depth_limit;
        RandomAccessIter cut = __unguarded_partition(first, last,
            T(__median(*first, *(first + (last - first) / 2), *(last - 1), comp)), comp);
        __introsort_loop(cut, last, (T*)0, depth_limit, comp);
        last = cut;
    }
}
static int Fail() { std::printf("00a35490_TEST FAIL\n"); return 1; }

int main()
{
    unsigned int keys[40] = { 31, 7, 99, 3, 58, 7, 12, 0x80000001u, 44, 2, 77, 19, 61, 8, 25, 90, 5, 33, 71, 14, 66, 1, 48, 27, 83, 9, 52, 38, 11, 95, 4, 60, 22, 87, 16, 41, 73, 6, 55, 29 };
    TEntry a[40]; Fill(a, keys, 40);
    int strings = g_strings;
    __introsort_loop(a, a + 40, (TEntry*)0, 8, CKeyPairCompareLess());
    if (g_strings != strings || !Consistent(a, 40)) return Fail();
    __insertion_sort(a, a + 40, CKeyPairCompareLess());
    if (!Sorted(a, 40) || !Consistent(a, 40)) return Fail();
    TEntry b[40]; Fill(b, keys, 40);
    __introsort_loop(b, b + 40, (TEntry*)0, 0, CKeyPairCompareLess());
    if (!Sorted(b, 40) || !Consistent(b, 40)) return Fail();
    std::printf("00a35490_TEST PASS\n");
    return 0;
}