// behaviour copy of STLport __insertion_sort over the CVectorMap<long, CCompositeSurfaceInfo> entries @ 0x00BEECD0
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#endif
// Behaviour model of the CVectorMap<long, CCompositeSurfaceInfo> STLport helpers (the entry is the key plus a
// polymorphic info holding a counted pointer; copies share the count block, assignments re-point it).
static int g_destroyed = 0;                  // count blocks whose data was destroyed
static int g_mallocs = 0, g_frees = 0;
static void* TestMalloc(unsigned int n) { ++g_mallocs; return std::malloc(n); }
static void TestFree(void* p) { if (p) { ++g_frees; std::free(p); } }

class CBaseClass {
public:
    virtual ~CBaseClass() { }
};
class CCPPointerInfo {
public:
    long  RefCount;
    void (__fastcall* DeleteFunction)(void* data);
    void* Data;
};
static void __fastcall TestDeleteData(void* data) { ++g_destroyed; (void)data; }

template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) { }
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
    ~CCountedPointer() { DeleteData(); Data = 0; Info = 0; }
    void DeleteData()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunction(Info->Data);
                operator delete(Info);
            }
        }
    }
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        CCPPointerInfo* info = other.Info;
        T* data = other.Data;
        if (Info != info) {
            DeleteData();
            Data = data;
            Info = info;
            if (info)
                ++info->RefCount;
        }
        return *this;
    }
};
class CCompositeSurfaceCache { public: int Id; };
class CCompositeSurfaceInfo : public CBaseClass {
public:
    CCountedPointer<CCompositeSurfaceCache> Cache;
};
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair() : first(), second() { }
    pair(const T1& a, const T2& b) : first(a), second(b) { }
};
typedef pair<long, CCompositeSurfaceInfo> CSurfaceEntry;
struct CSurfaceEntryLess {
    bool operator()(const CSurfaceEntry& a, const CSurfaceEntry& b) const { return a.first < b.first; }
};

// test fixture: entry i gets key keys[i] and its own count block (RefCount 1) holding cache object i
static CCompositeSurfaceCache g_caches[64];
static long g_cacheKey[64];
static void Setup(CSurfaceEntry* a, const long* keys, int n)
{
    for (int i = 0; i < n; ++i) {
        g_caches[i].Id = i;
        g_cacheKey[i] = keys[i];
        a[i].first = keys[i];
        CCPPointerInfo* info = new CCPPointerInfo;
        info->RefCount = 1;
        info->DeleteFunction = TestDeleteData;
        info->Data = &g_caches[i];
        a[i].second.Cache.Data = &g_caches[i];
        a[i].second.Cache.Info = info;
    }
}
// every entry still owns exactly one reference to a block whose cache carries the entry's key
static bool Intact(const CSurfaceEntry* a, int n)
{
    for (int i = 0; i < n; ++i) {
        const CCountedPointer<CCompositeSurfaceCache>& c = a[i].second.Cache;
        if (c.Info == 0 || c.Info->Data != c.Data || c.Info->RefCount != 1) return false;
        if (g_cacheKey[c.Data->Id] != a[i].first) return false;
    }
    return g_destroyed == 0;
}
static bool Sorted(const CSurfaceEntry* a, int n)
{
    for (int i = 1; i < n; ++i)
        if (a[i].first < a[i - 1].first) return false;
    return true;
}
static bool Distinct(const CSurfaceEntry* a, int n)   // every cache object appears once
{
    int seen[64];
    std::memset(seen, 0, sizeof(seen));
    for (int i = 0; i < n; ++i) {
        int id = a[i].second.Cache.Data->Id;
        if (seen[id]) return false;
        seen[id] = 1;
    }
    return true;
}

// STLport 4.6 algorithms (the reconstructed bodies plus their callees)
struct random_access_iterator_tag { };
template <class I1, class I2>
I2 __copy_backward(I1 first, I1 last, I2 result, const random_access_iterator_tag&, int*)
{
    for (int n = (int)(last - first); n > 0; --n)
        *--result = *--last;
    return result;
}
template <class I1, class I2> inline I2 copy_backward(I1 first, I1 last, I2 result)
{
    return __copy_backward(first, last, result, random_access_iterator_tag(), (int*)0);
}
template <class I1, class I2> void iter_swap(I1 a, I2 b) { CSurfaceEntry tmp = *a; *a = *b; *b = tmp; }
template <class R, class T, class C> void __unguarded_linear_insert(R last, T val, C comp)
{
    R next = last;
    --next;
    while (comp(val, *next)) {
        *last = *next;
        last = next;
        --next;
    }
    *last = val;
}
template <class R, class T, class C> void __linear_insert(R first, R last, T val, C comp)
{
    if (comp(val, *first)) {
        copy_backward(first, last, last + 1);
        *first = val;
    }
    else
        __unguarded_linear_insert(last, val, comp);
}
template <class R, class C> void __insertion_sort(R first, R last, C comp)
{
    if (first == last) return;
    for (R i = first + 1; i != last; ++i)
        __linear_insert(first, i, CSurfaceEntry(*i), comp);
}
template <class R, class T, class C> void __unguarded_insertion_sort_aux(R first, R last, T*, C comp)
{
    for (R i = first; i != last; ++i)
        __unguarded_linear_insert(i, T(*i), comp);
}
template <class R, class T, class C> R __unguarded_partition(R first, R last, T pivot, C comp)
{
    for (;;) {
        while (comp(*first, pivot))
            ++first;
        --last;
        while (comp(pivot, *last))
            --last;
        if (!(first < last))
            return first;
        iter_swap(first, last);
        ++first;
    }
}
template <class T, class C> const T& __median(const T& a, const T& b, const T& c, C comp)
{
    if (comp(a, b))
        if (comp(b, c)) return b;
        else if (comp(a, c)) return c;
        else return a;
    else if (comp(a, c)) return a;
    else if (comp(b, c)) return c;
    else return b;
}
template <class R, class D, class T, class C> void __push_heap(R first, D hole, D top, T val, C comp)
{
    D parent = (hole - 1) / 2;
    while (hole > top && comp(*(first + parent), val)) {
        *(first + hole) = *(first + parent);
        hole = parent;
        parent = (hole - 1) / 2;
    }
    *(first + hole) = val;
}
template <class R, class D, class T, class C> void __adjust_heap(R first, D hole, D len, T val, C comp)
{
    D top = hole;
    D second = 2 * hole + 2;
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
    __push_heap(first, hole, top, val, comp);
}
template <class R, class C, class T, class D> void __make_heap(R first, R last, C comp, T*, D*)
{
    if (last - first < 2) return;
    D len = (D)(last - first);
    D parent = (len - 2) / 2;
    for (;;) {
        __adjust_heap(first, parent, len, T(*(first + parent)), comp);
        if (parent == 0) return;
        parent--;
    }
}
template <class R, class T, class C, class D> void __pop_heap(R first, R last, R result, T val, C comp, D*)
{
    *result = *first;
    __adjust_heap(first, D(0), D(last - first), val, comp);
}
template <class R, class C> void sort_heap(R first, R last, C comp)
{
    while (last - first > 1) {
        --last;
        __pop_heap(first, last, last, CSurfaceEntry(*last), comp, (int*)0);
    }
}
template <class R, class T, class C> void __partial_sort(R first, R middle, R last, T*, C comp)
{
    __make_heap(first, middle, comp, (T*)0, (int*)0);
    for (R i = middle; i < last; ++i)
        if (comp(*i, *first))
            __pop_heap(first, middle, i, T(*i), comp, (int*)0);
    sort_heap(first, middle, comp);
}
template <class R, class T, class S, class C> void __introsort_loop(R first, R last, T*, S depth, C comp)
{
    while (last - first > 16) {
        if (depth == 0) {
            __partial_sort(first, last, last, (T*)0, comp);
            return;
        }
        --depth;
        R cut = __unguarded_partition(first, last,
                                      T(__median(*first, *(first + (last - first) / 2), *(last - 1), comp)), comp);
        __introsort_loop(cut, last, (T*)0, depth, comp);
        last = cut;
    }
}

// the entry vector (STLport layout) with the reconstructed growth paths
struct __false_type { };
template <class T> class CEntryVector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    CEntryVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CEntryVector() { _M_clear(); }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    unsigned int capacity() const { return (unsigned int)(_M_end_of_storage - _M_start); }
    T* allocate(unsigned int n) { return n != 0 ? (T*)TestMalloc(n * sizeof(T)) : 0; }
    static T* ucopy(const T* first, const T* last, T* result)
    {
        for (; first != last; ++first, ++result) new (result) T(*first);
        return result;
    }
    static T* ufill(T* first, unsigned int n, const T& x)
    {
        for (; n > 0; --n, ++first) new (first) T(x);
        return first;
    }
    void _M_clear()
    {
        for (T* p = _M_start; p != _M_finish; ++p) p->~T();
        TestFree(_M_start);
    }
    void _M_set(T* s, T* f, T* e) { _M_start = s; _M_finish = f; _M_end_of_storage = e; }
    void reserve(unsigned int n)
    {
        if (capacity() < n) {
            const unsigned int old_size = size();
            T* tmp;
            if (_M_start) {
                tmp = allocate(n);
                ucopy(_M_start, _M_finish, tmp);
                _M_clear();
            }
            else
                tmp = allocate(n);
            _M_set(tmp, tmp + old_size, tmp + n);
        }
    }
    void _M_insert_overflow(T* position, const T& x, const __false_type&, unsigned int fill_len, bool atend)
    {
        const unsigned int old_size = size();
        const unsigned int len = old_size + (old_size < fill_len ? fill_len : old_size);
        T* new_start = allocate(len);
        T* new_finish = ucopy(_M_start, position, new_start);
        if (fill_len == 1) {
            new (new_finish) T(x);
            ++new_finish;
        }
        else
            new_finish = ufill(new_finish, fill_len, x);
        if (!atend)
            new_finish = ucopy(position, _M_finish, new_finish);
        _M_clear();
        _M_set(new_start, new_finish, new_start + len);
    }
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) T(x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
};
class CSurfaceMap : public CEntryVector<CSurfaceEntry> {
public:
    unsigned char Compare;
    bool Dirty;
    CSurfaceMap() : Compare(0), Dirty(false) { }
    void Add(const long& key, const CCompositeSurfaceInfo& value)
    {
        push_back(CSurfaceEntry(key, value));
        Dirty = true;
    }
};
static const long kKeys[40] = { 17, 3, 29, 3, -5, 41, 0, 12, 8, 33, 21, -9, 6, 14, 27, 2, 39, 11, 25, 30,
                                7, 19, -1, 36, 4, 22, 16, 9, 31, 1, 28, 13, 35, 5, 20, 24, 10, 38, 15, 26 };

int main()
{
    int ok = 1;
    CSurfaceEntryLess comp;
    (void)comp;

    CSurfaceEntry a[40];
    Setup(a, kKeys, 40);
    __insertion_sort(a, a + 40, comp);
    if (!Sorted(a, 40) || !Intact(a, 40) || !Distinct(a, 40)) ok = 0;
    if (a[0].first != -9 || a[39].first != 41) ok = 0;
    if (ok) std::printf("00beecd0_TEST PASS\n");
    else { std::printf("00beecd0_TEST FAIL\n"); return 1; }
    return 0;
}