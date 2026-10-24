// Behaviour test for 006b5a50: std::lower_bound over sorted pair<long,float> with the STLport lexicographic pair less (model copy, with a __lower_bound stand-in).
#include <cstdio>
template <class T1, class T2> struct pair { T1 first; T2 second; };
template <class T1, class T2> inline bool operator<(const pair<T1, T2>& x, const pair<T1, T2>& y)
{ return x.first < y.first || (!(y.first < x.first) && x.second < y.second); }
template <class T> struct less { bool operator()(const T& x, const T& y) const { return x < y; } };
template <class T> inline less<T> __less(T*) { return less<T>(); }
template <class It, class Tp, class Cmp, class Dist>
It __lower_bound(It first, It last, const Tp& val, Cmp comp, Dist*)
{   // stand-in for 0x006B5890 (STLport bisection)
    Dist len = last - first;
    while (len > 0) { Dist half = len >> 1; It middle = first + half;
        if (comp(*middle, val)) { first = middle + 1; len = len - half - 1; } else len = half; }
    return first;
}
template <class It, class Tp> It lower_bound(It first, It last, const Tp& val)
{ return __lower_bound(first, last, val, __less((Tp*)0), (int*)0); }
int main()
{
    pair<long, float> a[5] = { {1, 0.5f}, {2, 0.0f}, {2, 1.0f}, {4, 0.25f}, {9, 2.0f} };
    pair<long, float> k1 = {2, 0.5f}, k2 = {0, 9.0f}, k3 = {10, 0.0f}, k4 = {4, 0.25f};
    int fail = 0;
    if (lower_bound(a, a + 5, k1) != a + 2) fail = 1;   // between (2,0) and (2,1)
    if (lower_bound(a, a + 5, k2) != a) fail = 1;
    if (lower_bound(a, a + 5, k3) != a + 5) fail = 1;
    if (lower_bound(a, a + 5, k4) != a + 3) fail = 1;   // an equal element is found, not passed
    if (fail) { std::printf("006b5a50_TEST FAIL\n"); return 1; }
    std::printf("006b5a50_TEST PASS\n");
    return 0;
}