// behaviour test for 0x004DFDA6 copy_backward (model: the range is copied ending at result, the new begin returned)
#include <cstdio>
#include <algorithm>
int main()
{
    int a[6] = { 1, 2, 3, 0, 0, 0 };
    int* r = std::copy_backward(a, a + 3, a + 5);
    int ok = r == a + 2 && a[2] == 1 && a[3] == 2 && a[4] == 3;
    if (!ok) { std::printf("004dfda6_TEST FAIL\n"); return 1; }
    std::printf("004dfda6_TEST PASS\n");
    return 0;
}