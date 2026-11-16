#include <cstdio>
int __fastcall GetMaxDistFromCustomer()
{
    return 10;
}
int main()
{
    if (GetMaxDistFromCustomer() == 10) { std::printf("AUTO_TINY_00798d20_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00798d20_TEST FAIL\n");
    return 1;
}