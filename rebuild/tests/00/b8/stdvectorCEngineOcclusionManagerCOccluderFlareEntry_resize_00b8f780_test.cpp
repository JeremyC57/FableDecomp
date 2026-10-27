#include <cstdio>
#include <vector>
// Behaviour model: resize(n, x) shrinks by erasing the tail, grows by appending copies of x.
struct E { unsigned char b[7]; };
int main()
{
    std::vector<E> v;
    E x = { { 1, 2, 3, 4, 5, 6, 7 } };
    v.resize(3, x);
    if (v.size() != 3 || v[2].b[6] != 7) { std::printf("00b8f780_TEST FAIL\n"); return 1; }
    E y = { { 9, 9, 9, 9, 9, 9, 9 } };
    v.resize(1, y);
    if (v.size() != 1 || v[0].b[0] != 1) { std::printf("00b8f780_TEST FAIL\n"); return 1; }
    v.resize(2, y);
    if (v.size() != 2 || v[1].b[3] != 9 || v[0].b[3] != 4) { std::printf("00b8f780_TEST FAIL\n"); return 1; }
    std::printf("00b8f780_TEST PASS\n");
    return 0;
}