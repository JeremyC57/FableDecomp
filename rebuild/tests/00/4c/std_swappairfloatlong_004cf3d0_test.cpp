// test for 004cf3d0 swap<pair<float,long> >
#include <stdio.h>
namespace std {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair() {}
    pair(const pair& p) : first(p.first), second(p.second) {}
};
template <class T> void __fastcall swap(T& a, T& b)
{
    T tmp = a;
    a = b;
    b = tmp;
}
template void __fastcall swap<pair<float, long> >(pair<float, long>&, pair<float, long>&);
}

int main()
{
    std::pair<float, long> a, b;
    a.first = 1.5f; a.second = 7; b.first = -2.25f; b.second = 9;
    std::swap(a, b);
    int ok = a.first == -2.25f && a.second == 9 && b.first == 1.5f && b.second == 7 && sizeof(a) == 8;
    std::swap(a, a);
    ok &= a.first == -2.25f && a.second == 9;
    printf(ok ? "004cf3d0_TEST PASS\n" : "004cf3d0_TEST FAIL\n");
    return ok ? 0 : 1;
}