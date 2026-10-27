// behaviour test for 0x0097ADA0 __unguarded_linear_insert (model: val moves down past the larger elements)
#include <cstdio>
int main()
{
    int a[5] = { 1, 3, 5, 7, 4 };
    int* last = a + 4; int val = *last; int* next = last - 1;
    while (val < *next) { *last = *next; last = next; --next; }
    *last = val;
    int ok = a[0] == 1 && a[1] == 3 && a[2] == 4 && a[3] == 5 && a[4] == 7;
    if (!ok) { std::printf("0097ada0_TEST FAIL\n"); return 1; }
    std::printf("0097ada0_TEST PASS\n");
    return 0;
}