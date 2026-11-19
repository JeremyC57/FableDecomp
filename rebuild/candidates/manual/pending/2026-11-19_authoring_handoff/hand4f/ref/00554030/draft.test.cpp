// behaviour test for 0x00554030: operator[] returns the existing value or inserts an empty one
#include <cstdio>
#include <map>
int main()
{
    std::map<long, void*> m; int x; m[5] = &x;
    void*& a = m[5]; void*& b = m[7];
    if (a != &x || b != 0 || m.size() != 2) { std::printf("00554030_TEST FAIL\n"); return 1; }
    std::printf("00554030_TEST PASS\n");
    return 0;
}
