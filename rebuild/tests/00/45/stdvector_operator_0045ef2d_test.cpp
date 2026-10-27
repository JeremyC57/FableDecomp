// behaviour test for 0x0045EF2D: assignment copies the elements whether the target grows, shrinks or extends
#include <cstdio>
#include <vector>
int main()
{
    std::vector<float> a(5, (float)3), b(2, (float)1), c(9, (float)7);
    b = a; int ok = b.size() == 5 && b[4] == (float)3;
    c = a; ok = ok && c.size() == 5 && c[0] == (float)3;
    std::vector<float> d; d.reserve(8); d.push_back((float)2); d = a; ok = ok && d.size() == 5 && d[2] == (float)3;
    a = a; ok = ok && a.size() == 5;
    if (!ok) { std::printf("0045ef2d_TEST FAIL\n"); return 1; }
    std::printf("0045ef2d_TEST PASS\n");
    return 0;
}