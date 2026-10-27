// behaviour test for 0x005B3167: operator[] returns the existing value or inserts a zero one
#include <cstdio>
#include <map>
int main()
{
    std::map<unsigned long, long> m; m[5] = 3;
    long& a = m[5]; long& b = m[7]; ++m[7];
    if (a != 3 || b != 1 || m.size() != 2) { std::printf("005b3167_TEST FAIL\n"); return 1; }
    std::printf("005b3167_TEST PASS\n");
    return 0;
}