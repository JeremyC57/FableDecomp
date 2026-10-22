#include <cstdio>
void __stdcall CEngineInternalPrimitiveBase_Update(int primitive, int arg1) {}
int main()
{
    CEngineInternalPrimitiveBase_Update(1, 2);
    std::printf("AUTO_TINY_00b8fd80_TEST PASS\n");
    return 0;
}