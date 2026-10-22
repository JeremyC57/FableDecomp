#include <cstdio>
void __stdcall CIEngine_AreaChanged(int boxI) {}
int main()
{
    CIEngine_AreaChanged(1);
    std::printf("AUTO_TINY_00b23520_TEST PASS\n");
    return 0;
}