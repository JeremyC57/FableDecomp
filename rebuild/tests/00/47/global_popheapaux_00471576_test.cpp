// Behaviour test for 00471576: __pop_heap_aux for pair<long, EHeroAbility> -- heap-sort seven keyed elements through the row's function (with the
// SGI __adjust_heap/__push_heap written out here) and check ascending keys with each payload still beside its key.
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __cdecl
#endif
static const char* const g_names[10] = { "k0", "k1", "k2", "k3", "k4", "k5", "k6", "k7", "k8", "k9" };
#define NUM(s) ((long)((s).c_str()[1] - '0'))
template <class T, class C> void push_impl(T* first, long holeIndex, long topIndex, T value, C comp)
{
    long parent = (holeIndex - 1) / 2;
    while (holeIndex > topIndex && comp(first[parent], value)) {
        first[holeIndex] = first[parent];
        holeIndex = parent;
        parent = (holeIndex - 1) / 2;
    }
    first[holeIndex] = value;
}
template <class T, class C> void adjust_impl(T* first, long holeIndex, long len, T value, C comp)
{
    long topIndex = holeIndex;
    long secondChild = 2 * holeIndex + 2;
    while (secondChild < len) {
        if (comp(first[secondChild], first[secondChild - 1]))
            secondChild--;
        first[holeIndex] = first[secondChild];
        holeIndex = secondChild;
        secondChild = 2 * (secondChild + 1);
    }
    if (secondChild == len) {
        first[holeIndex] = first[secondChild - 1];
        holeIndex = secondChild - 1;
    }
    push_impl(first, holeIndex, topIndex, value, comp);
}
enum EHeroAbility { HERO_ABILITY_NULL = 0 };

struct PairLongHeroAbility {                            // std::pair<long, EHeroAbility>, 8 bytes
    long         first;                                 // +0x00
    EHeroAbility second;                                // +0x04
    PairLongHeroAbility() { }
    PairLongHeroAbility(const PairLongHeroAbility& other) : first(other.first), second(other.second) { }
};

struct CKeyPairCompareLess {                            // CKeyPairCompareLess<long, EHeroAbility>, empty
    CKeyPairCompareLess() { }
    bool operator()(const PairLongHeroAbility& a, const PairLongHeroAbility& b) const { return a.first < b.first; }
};

void __adjust_heap(PairLongHeroAbility* first, long holeIndex, long len, PairLongHeroAbility value,
                              CKeyPairCompareLess comp) { adjust_impl(first, holeIndex, len, value, comp); }   // 0x004714AE

inline void __pop_heap(PairLongHeroAbility* first, PairLongHeroAbility* last, PairLongHeroAbility* result,
                                  PairLongHeroAbility value, CKeyPairCompareLess comp, long*)
{
    *result = *first;
    __adjust_heap(first, 0, long(last - first), value, comp);
}

void __pop_heap_aux(PairLongHeroAbility* first, PairLongHeroAbility* last, PairLongHeroAbility*,
                               CKeyPairCompareLess comp)
{
    __pop_heap(first, last - 1, last - 1, PairLongHeroAbility(*(last - 1)), comp, (long*)0);
}

#define KEY(e) ((long)(e).first)
int main()
{
    {
        static const int keys[7] = { 5, 9, 1, 7, 3, 8, 2 };
        static const int sorted[7] = { 1, 2, 3, 5, 7, 8, 9 };
        PairLongHeroAbility a[7];
        for (int i = 0; i < 7; ++i) {
            PairLongHeroAbility& e = a[i];
            int k = keys[i];
            e.first = k;
            e.second = (EHeroAbility)(k * 10);
        }
        CKeyPairCompareLess comp;
        for (int i = 1; i < 7; ++i) {
            PairLongHeroAbility v(a[i]);
            push_impl(a, (long)i, 0L, v, comp);
        }
        for (int len = 7; len > 1; --len)
            __pop_heap_aux(a, a + len, (PairLongHeroAbility*)0, comp);
        for (int i = 0; i < 7; ++i) {
            const PairLongHeroAbility& e = a[i];
            if (KEY(e) != sorted[i] || !((long)e.second == KEY(e) * 10)) { std::printf("00471576_TEST FAIL\n"); return 1; }
        }
    }
    std::printf("00471576_TEST PASS\n");
    return 0;
}