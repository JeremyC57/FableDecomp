#include <cstdio>
void __stdcall CIEngine_SetPrimitivePosition(int handle, int position) {}
int main()
{
    CIEngine_SetPrimitivePosition(1, 2);
    std::printf("AUTO_TINY_00b233b0_TEST PASS\n");
    return 0;
}