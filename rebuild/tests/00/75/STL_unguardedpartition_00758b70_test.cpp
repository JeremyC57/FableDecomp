// Behaviour test for 0x00758B70 _STL::__unguarded_partition over the 0x2C-byte wound-morph entries
#include <cstdio>

struct CWoundMorphsEntry { long Words[10]; };
template <class A, class B> struct pair {
    A first;
    B second;
    pair() {}
    pair(const pair& other) : first(other.first), second(other.second) {}
};
typedef pair<int, CWoundMorphsEntry> TWoundPair;
struct CKeyPairCompareLess {
    bool operator()(const TWoundPair& a, const TWoundPair& b) const { return a.first < b.first; }
};

template <class T> inline void __iter_swap(T* a, T* b) { T tmp = *a; *a = *b; *b = tmp; }

TWoundPair* __unguarded_partition(TWoundPair* first, TWoundPair* last, TWoundPair pivot, CKeyPairCompareLess comp)
{
    for (;;) {
        while (comp(*first, pivot))
            ++first;
        --last;
        while (comp(pivot, *last))
            --last;
        if (!(first < last))
            return first;
        __iter_swap(first, last);
        ++first;
    }
}

int main()
{
    int keys[9] = { 7, 2, 9, 4, 4, 1, 8, 3, 6 };
    TWoundPair a[9];
    for (int i = 0; i < 9; ++i) { a[i].first = keys[i]; for (int k = 0; k < 10; ++k) a[i].second.Words[k] = keys[i] * 10 + k; }
    TWoundPair pivot = a[3];                  // key 4 (present in the range, so the scans are guarded)
    TWoundPair* cut = __unguarded_partition(a, a + 9, pivot, CKeyPairCompareLess());
    int ok = 1;
    if (cut <= a || cut >= a + 9) ok = 0;
    for (TWoundPair* p = a; p < cut; ++p) if (p->first > 4) ok = 0;
    for (TWoundPair* p = cut; p < a + 9; ++p) if (p->first < 4) ok = 0;
    int sum = 0;
    for (int i = 0; i < 9; ++i) { sum += a[i].first; for (int k = 0; k < 10; ++k) if (a[i].second.Words[k] != a[i].first * 10 + k) ok = 0; }
    if (sum != 44) ok = 0;
    if (!ok) { std::printf("00758b70_TEST FAIL\n"); return 1; }
    std::printf("00758b70_TEST PASS\n");
    return 0;
}