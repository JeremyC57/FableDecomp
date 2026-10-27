// behaviour test for 0x0045BC09: assignment copies the elements whether the target grows, shrinks or extends
#include <cstdio>
#include <vector>
int main()
{
    std::vector<unsigned long> a(5, (unsigned long)3), b(2, (unsigned long)1), c(9, (unsigned long)7);
    b = a; int ok = b.size() == 5 && b[4] == (unsigned long)3;
    c = a; ok = ok && c.size() == 5 && c[0] == (unsigned long)3;
    std::vector<unsigned long> d; d.reserve(8); d.push_back((unsigned long)2); d = a; ok = ok && d.size() == 5 && d[2] == (unsigned long)3;
    a = a; ok = ok && a.size() == 5;
    if (!ok) { std::printf("0045bc09_TEST FAIL\n"); return 1; }
    std::printf("0045bc09_TEST PASS\n");
    return 0;
}