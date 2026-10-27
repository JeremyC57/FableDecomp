#include <cstdio>
#include <vector>
// Behaviour model: resize shrinks and grows with the fill value.
struct E { unsigned long a; long b, c; };
int main()
{
    std::vector<E> v; E x = { 7, 8, 9 };
    v.resize(5, x);
    if (v.size() != 5 || v[4].a != 7) { std::printf("0086d900_TEST FAIL\n"); return 1; }
    v.resize(2, x);
    if (v.size() != 2) { std::printf("0086d900_TEST FAIL\n"); return 1; }
    std::printf("0086d900_TEST PASS\n");
    return 0;
}