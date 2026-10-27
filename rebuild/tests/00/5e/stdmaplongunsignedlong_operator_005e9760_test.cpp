// behaviour test for 0x005E9760: operator[] returns the existing value or inserts a zero one
#include <cstdio>
#include <map>
int main()
{
    std::map<long, unsigned long> m; m[-3] = 9;
    unsigned long& a = m[-3]; unsigned long& b = m[4];
    if (a != 9 || b != 0 || m.size() != 2) { std::printf("005e9760_TEST FAIL\n"); return 1; }
    std::printf("005e9760_TEST PASS\n");
    return 0;
}