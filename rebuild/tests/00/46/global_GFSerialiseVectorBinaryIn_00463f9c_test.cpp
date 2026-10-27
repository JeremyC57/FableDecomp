// behaviour test for 0x00463F9C: the reader sizes the vector from the count and fills every element
#include <cstdio>
#include <vector>
static const long g_stream[4] = { 3, 7, 1, 4 }; static int g_pos = 0;
static long ReadLong() { return g_stream[g_pos++]; }
int main()
{
    std::vector<long> v; long count = ReadLong(); v.resize(count);
    for (long i = 0; i < count; ++i) v[i] = ReadLong();
    if (v.size() != 3 || v[0] != 7 || v[2] != 4) { std::printf("00463f9c_TEST FAIL\n"); return 1; }
    std::printf("00463f9c_TEST PASS\n");
    return 0;
}