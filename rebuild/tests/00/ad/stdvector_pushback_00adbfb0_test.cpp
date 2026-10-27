// behaviour test for 0x00ADBFB0: push_back appends 12-byte records in order, growing when full
#include <cstdio>
#include <vector>
struct E { long a, b, c; };
int main()
{
    std::vector<E> v;
    for (long i = 0; i < 7; ++i) { E e = { i, i * 2, i * 3 }; v.push_back(e); }
    if (v.size() != 7 || v[6].c != 18 || v[3].b != 6 || v[0].a != 0) { std::printf("00adbfb0_TEST FAIL\n"); return 1; }
    std::printf("00adbfb0_TEST PASS\n");
    return 0;
}