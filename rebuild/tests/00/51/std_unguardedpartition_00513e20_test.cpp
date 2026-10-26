// behaviour test for 0x00513E20 std::__unguarded_partition over pointer elements: everything left of the returned cursor compares
// no later than the pivot, everything from it on no earlier, the elements are only permuted, and the comparator's result is used as
// a truth value
#include <cstdio>
struct Item { int Key; };
struct Compare {
    bool operator()(const Item* a, const Item* b) const { return a->Key < b->Key; }
};
static Item** Partition(Item** first, Item** last, Item* pivot, Compare comp)
{
    for (;;) {
        while (comp(*first, pivot)) ++first;
        --last;
        while (comp(pivot, *last)) --last;
        if (!(first < last)) return first;
        Item* t = *first; *first = *last; *last = t;
        ++first;
    }
}
int main()
{
    int fail = 0;
    static const int keys[9] = { 7, 2, 9, 4, 5, 1, 8, 3, 6 };
    Item items[9]; Item* v[9];
    for (int i = 0; i < 9; ++i) { items[i].Key = keys[i]; v[i] = &items[i]; }
    Compare c;
    Item** cut = Partition(v, v + 9, &items[4], c);   // pivot key 5
    int k = (int)(cut - v);
    if (k <= 0 || k >= 9) fail |= 1;
    for (int i = 0; i < 9; ++i) {
        if (i < k && v[i]->Key > 5) fail |= 2;
        if (i >= k && v[i]->Key < 5) fail |= 4;
    }
    int seen = 0;
    for (int i = 0; i < 9; ++i) seen |= 1 << v[i]->Key;
    if (seen != 0x3FE) fail |= 8;
    if (fail) { std::printf("00513e20_TEST FAIL %d\n", fail); return 1; }
    std::printf("00513e20_TEST PASS\n");
    return 0;
}