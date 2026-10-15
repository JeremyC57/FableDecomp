#include <cstdio>
#include <cstring>
// Behaviour of 0x00731300 (chunk 901 piece): slot 23 is constant true
struct CTCDoor_Calls { bool vfunc_23(); };
bool CTCDoor_Calls::vfunc_23() { return true; }
int main()
{
    CTCDoor_Calls o;
    if (!o.vfunc_23()) { std::printf("00731300_TEST FAIL\n"); return 1; }
    std::printf("00731300_TEST PASS\n"); return 0;
}
