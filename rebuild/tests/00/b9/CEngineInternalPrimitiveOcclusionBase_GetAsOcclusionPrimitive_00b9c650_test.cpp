#include <cstdio>
void* __fastcall CEngineInternalPrimitiveOcclusionBase_GetAsOcclusionPrimitive(void* occlusion)
{
    return occlusion;
}
int main()
{
    int value = 0;
    if (CEngineInternalPrimitiveOcclusionBase_GetAsOcclusionPrimitive(&value) == &value) { std::printf("AUTO_TINY_00b9c650_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00b9c650_TEST FAIL\n");
    return 1;
}