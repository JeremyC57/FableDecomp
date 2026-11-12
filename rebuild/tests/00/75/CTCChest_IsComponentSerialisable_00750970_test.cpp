#include <cstdio>
#include <cstring>
// Behaviour of 0x00750970 (chunk 901 piece): slot 23 is constant true
struct CTCChest_Calls { bool vfunc_23(); };
bool CTCChest_Calls::vfunc_23() { return true; }
int main()
{
    CTCChest_Calls o;
    if (!o.vfunc_23()) { std::printf("00750970_TEST FAIL\n"); return 1; }
    std::printf("00750970_TEST PASS\n"); return 0;
}