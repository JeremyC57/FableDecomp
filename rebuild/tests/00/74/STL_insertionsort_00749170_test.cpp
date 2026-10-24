// Behaviour test for 0x00749170 _STL::__insertion_sort(CThing**, CThing**, CCompare_FurthestFromInitPos2D)
#include <cstdio>
#include <cstring>

struct CThing { int key; };

struct CCompare_FurthestFromInitPos2D {
    bool operator()(CThing* a, CThing* b) const { return a->key > b->key; }   // stand-in ordering: larger key first
};

inline void* __copy_trivial_backward(const void* first, const void* last, void* result)
{
    const int num = (const char*)last - (const char*)first;
    return (num > 0) ? std::memmove((char*)result - num, first, num) : result;
}
inline CThing** copy_backward(CThing** first, CThing** last, CThing** result)
{
    return (CThing**)__copy_trivial_backward(first, last, result);
}

static int g_unguardedCalls = 0;
void __unguarded_linear_insert(CThing** last, CThing* val, CCompare_FurthestFromInitPos2D comp)
{
    ++g_unguardedCalls;
    CThing** next = last;
    --next;
    while (comp(val, *next)) {
        *last = *next;
        last = next;
        --next;
    }
    *last = val;
}

template <class RandomAccessIter, class T, class Compare>
inline void __linear_insert(RandomAccessIter first, RandomAccessIter last, T*, const Compare& comp)
{
    T val = *last;
    if (comp(val, *first)) {
        copy_backward(first, last, last + 1);
        *first = val;
    }
    else
        __unguarded_linear_insert(last, val, comp);
}

void __insertion_sort(CThing** first, CThing** last, CCompare_FurthestFromInitPos2D comp)
{
    if (first == last)
        return;
    for (CThing** i = first + 1; i != last; ++i)
        __linear_insert(first, i, (CThing**)0, comp);
}

int main()
{
    CThing t[7];
    int keys[7] = { 4, 9, 1, 9, 12, 0, 5 };
    CThing* p[7];
    for (int i = 0; i < 7; ++i) { t[i].key = keys[i]; p[i] = &t[i]; }
    __insertion_sort(p, p + 7, CCompare_FurthestFromInitPos2D());
    int want[7] = { 12, 9, 9, 5, 4, 1, 0 };
    int ok = 1;
    for (int i = 0; i < 7; ++i) if (p[i]->key != want[i]) ok = 0;
    // stable for equal keys: the first 9 (t[1]) stays ahead of the second (t[3])
    if (p[1] != &t[1] || p[2] != &t[3]) ok = 0;
    if (g_unguardedCalls == 0) ok = 0;
    __insertion_sort(p, p, CCompare_FurthestFromInitPos2D());        // empty range
    CThing* one[1] = { &t[2] };
    __insertion_sort(one, one + 1, CCompare_FurthestFromInitPos2D());
    if (one[0] != &t[2]) ok = 0;
    if (!ok) { std::printf("00749170_TEST FAIL\n"); return 1; }
    std::printf("00749170_TEST PASS\n");
    return 0;
}