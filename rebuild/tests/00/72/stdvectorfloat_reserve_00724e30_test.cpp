// behaviour test for 0x00724E30: reserve keeps the elements and grows the capacity only when needed
#include <cstdio>
#include <vector>
int main()
{
    std::vector<long> v; v.reserve(5);
    for (long i = 0; i < 3; ++i) v.push_back(i * 7);
    v.reserve(2);
    v.reserve(40);
    if (v.capacity() < 40 || v.size() != 3 || v[2] != 14) { std::printf("00724e30_TEST FAIL\n"); return 1; }
    std::printf("00724e30_TEST PASS\n");
    return 0;
}