// behaviour test for 0x00454886: assignment copies the elements whether the target grows, shrinks or extends
#include <cstdio>
#include <vector>
int main()
{
    std::vector<long> a(5, (long)3), b(2, (long)1), c(9, (long)7);
    b = a; int ok = b.size() == 5 && b[4] == (long)3;
    c = a; ok = ok && c.size() == 5 && c[0] == (long)3;
    std::vector<long> d; d.reserve(8); d.push_back((long)2); d = a; ok = ok && d.size() == 5 && d[2] == (long)3;
    a = a; ok = ok && a.size() == 5;
    if (!ok) { std::printf("00454886_TEST FAIL\n"); return 1; }
    std::printf("00454886_TEST PASS\n");
    return 0;
}