// Behaviour test for 0x00748C10 _STL::__unguarded_insertion_sort_aux(CThing**, CThing**, CThing**, CCompare_FurthestFromInitPos2D)
#include <cstdio>

struct CThing { int key; };

struct CCompare_FurthestFromInitPos2D {
    bool operator()(CThing* a, CThing* b) const { return a->key > b->key; }   // stand-in ordering: larger key first
};

template <class RandomAccessIter, class T, class Compare>
inline void __unguarded_linear_insert(RandomAccessIter last, T val, Compare comp)
{
    RandomAccessIter next = last;
    --next;
    while (comp(val, *next)) {
        *last = *next;
        last = next;
        --next;
    }
    *last = val;
}

void __unguarded_insertion_sort_aux(CThing** first, CThing** last, CThing**, CCompare_FurthestFromInitPos2D comp)
{
    for (CThing** i = first; i != last; ++i)
        __unguarded_linear_insert(i, *i, comp);
}

int main()
{
    // p[0] is the guard (largest key) that stops every unguarded walk; sort [p+1, p+8)
    CThing t[8];
    int keys[8] = { 100, 4, 9, 1, 9, 12, 0, 5 };
    CThing* p[8];
    for (int i = 0; i < 8; ++i) { t[i].key = keys[i]; p[i] = &t[i]; }
    __unguarded_insertion_sort_aux(p + 1, p + 8, (CThing**)0, CCompare_FurthestFromInitPos2D());
    int want[8] = { 100, 12, 9, 9, 5, 4, 1, 0 };
    int ok = 1;
    for (int i = 0; i < 8; ++i) if (p[i]->key != want[i]) ok = 0;
    if (p[0] != &t[0]) ok = 0;
    if (p[2] != &t[2] || p[3] != &t[4]) ok = 0;            // equal keys keep their order
    __unguarded_insertion_sort_aux(p + 3, p + 3, (CThing**)0, CCompare_FurthestFromInitPos2D());   // empty range
    for (int i = 0; i < 8; ++i) if (p[i]->key != want[i]) ok = 0;
    if (!ok) { std::printf("00748c10_TEST FAIL\n"); return 1; }
    std::printf("00748c10_TEST PASS\n");
    return 0;
}