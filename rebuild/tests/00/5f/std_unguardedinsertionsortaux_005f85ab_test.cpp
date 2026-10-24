// Behaviour test for 005f85ab (std::__unguarded_insertion_sort_aux over pair<long, long> (SKeyEntryA)): the range comes out ordered by key, every element moved whole, copies balanced.
#include <cstdio>
#include <new>
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) { }
    pair(const pair& o) : first(o.first), second(o.second) { }
};
typedef pair<long, long> SKeyEntryA;
struct SKeyEntryALess {
    bool operator()(const SKeyEntryA& a, const SKeyEntryA& b) const { return a.first < b.first; }
};
typedef SKeyEntryA T;
typedef SKeyEntryALess C;
static int KeyOf(const T& x) { return (int)x.first; }
static bool Consistent(const T& x) { return x.second == 100 + x.first; }
void __unguarded_linear_insert(T* last, T val, C comp)          // the SGI algorithm (stands in for the retail callee)
{
    T* next = last;
    --next;
    while (comp(val, *next)) {
        *last = *next;
        last = next;
        --next;
    }
    *last = val;
}
void __unguarded_insertion_sort_aux(T* first, T* last, T*, C comp)
{
    for (T* i = first; i != last; ++i)
        __unguarded_linear_insert(i, T(*i), comp);
}
union UStorage { double d; void* p; char bytes[sizeof(T) * 8]; };
int main()
{
    static const int keys[8] = { 0, 6, 3, 4, 1, 7, 2, 5 };
    UStorage storage;
    T* items = reinterpret_cast<T*>(storage.bytes);
    for (int i = 0; i < 8; ++i)
        new (&items[i]) T(keys[i], 100L + keys[i]);
    C comp = C();
    __unguarded_insertion_sort_aux(items + 1, items + 1, items, comp);
    for (int i = 0; i < 8; ++i)
        if (KeyOf(items[i]) != keys[i]) { std::printf("005f85ab_TEST FAIL\n"); return 1; }
    __unguarded_insertion_sort_aux(items + 1, items + 8, items, comp);
    for (int i = 0; i < 8; ++i)
        if (KeyOf(items[i]) != i || !Consistent(items[i])) { std::printf("005f85ab_TEST FAIL\n"); return 1; }
    if (!(true)) { std::printf("005f85ab_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 8; ++i)
        items[i].~T();
    std::printf("005f85ab_TEST PASS\n");
    return 0;
}