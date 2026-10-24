// behaviour test for 0x00AE5AB0 __unguarded_linear_insert over pair<long, unsigned long>
#include <cstdio>
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair() { }
    pair(T1 a, T2 b) : first(a), second(b) { }
    pair(const pair& o) : first(o.first), second(o.second) { }
};
template <class T1, class T2>
inline bool operator<(const pair<T1, T2>& x, const pair<T1, T2>& y)
{
    return x.first < y.first || (!(y.first < x.first) && x.second < y.second);
}
template <class T> struct less {
    bool operator()(const T& x, const T& y) const { return x < y; }
};
template <class It, class T, class C>
void unguarded_linear_insert(It last, T val, C comp)
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
typedef pair<long, unsigned int> SPair;
int main()
{
    SPair a[5];
    a[0] = SPair(-5, 0); a[1] = SPair(1, 2); a[2] = SPair(1, 7); a[3] = SPair(3, 0); a[4] = SPair(1, 5);
    unguarded_linear_insert(a + 4, SPair(a[4]), less<SPair>());
    long ef[5] = { -5, 1, 1, 1, 3 };
    unsigned int es[5] = { 0, 2, 5, 7, 0 };
    for (int i = 0; i < 5; ++i)
        if (a[i].first != ef[i] || a[i].second != es[i]) { std::printf("00ae5ab0_TEST FAIL\n"); return 1; }
    // already in place: nothing moves
    SPair b[3];
    b[0] = SPair(0, 1); b[1] = SPair(2, 2); b[2] = SPair(2, 3);
    unguarded_linear_insert(b + 2, SPair(b[2]), less<SPair>());
    if (b[2].second != 3 || b[1].second != 2) { std::printf("00ae5ab0_TEST FAIL\n"); return 1; }
    std::printf("00ae5ab0_TEST PASS\n");
    return 0;
}