#include <cstdio>
bool __fastcall CanStartAttractMode()
{
    return false;
}
int main()
{
    if (CanStartAttractMode() == false) { std::printf("AUTO_TINY_00595a03_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00595a03_TEST FAIL\n");
    return 1;
}