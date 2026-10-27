// behaviour test for 0x00674400: assignment copies every 12-byte element, growing, shrinking or extending as needed
#include <cstdio>
#include <vector>
struct E { long a, b, c; };
int main()
{
    std::vector<E> x, y, z;
    for (long i = 0; i < 5; ++i) { E e = { i, i + 1, i + 2 }; x.push_back(e); }
    y = x;                                    // grow
    E big = { 9, 9, 9 }; for (int i = 0; i < 8; ++i) z.push_back(big);
    z = x;                                    // shrink in place
    y = y;                                    // self
    if (y.size() != 5 || y[4].c != 6 || z.size() != 5 || z[2].a != 2) { std::printf("00674400_TEST FAIL\n"); return 1; }
    std::printf("00674400_TEST PASS\n");
    return 0;
}