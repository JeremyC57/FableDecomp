// behaviour test for 00c08530 (standalone copy of the model)
#include <cstdio>

int lzo_assert_(int expr) { return (expr) ? 1 : 0; }

int main()
{
    if (!(lzo_assert_(0) == 0 && lzo_assert_(5) == 1 && lzo_assert_(-1) == 1)) { std::printf("00c08530_TEST FAIL\n"); return 1; }
    std::printf("00c08530_TEST PASS\n");
    return 0;
}