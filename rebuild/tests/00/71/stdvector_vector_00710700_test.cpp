// Behaviour test for 0x00710700 std::vector<short> copy ctor: an exact, independent copy (standalone copy of the logic)
#include <cstdio>
#include <vector>

int main()
{
    std::vector<short> a; for (short i = 0; i < 5; ++i) a.push_back((short)(i * 7 - 3));
    std::vector<short> b(a); if (b.size() != 5 || b[0] != -3 || b[4] != 25) { std::printf("00710700_TEST FAIL\n"); return 1; }
    b[0] = 1; if (a[0] != -3) { std::printf("00710700_TEST FAIL\n"); return 1; }
    std::vector<short> e; std::vector<short> f(e); if (!f.empty()) { std::printf("00710700_TEST FAIL\n"); return 1; }
    std::printf("00710700_TEST PASS\n");
    return 0;
}