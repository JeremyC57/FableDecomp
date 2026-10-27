#include <cstdio>
#include <vector>
// Behaviour model: resize(n) grows with default elements and shrinks by dropping the tail.
struct D { long n; std::vector<int> v; D() : n(0) { } };
int main()
{
    std::vector<D> a; a.resize(3);
    if (a.size() != 3 || a[2].n != 0) { std::printf("00f34bf0_TEST FAIL\n"); return 1; }
    a.resize(1);
    if (a.size() != 1) { std::printf("00f34bf0_TEST FAIL\n"); return 1; }
    std::printf("00f34bf0_TEST PASS\n");
    return 0;
}