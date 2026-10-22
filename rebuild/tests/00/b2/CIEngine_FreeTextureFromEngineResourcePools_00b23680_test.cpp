#include <cstdio>
void __stdcall CIEngine_FreeTextureFromEngineResourcePools(int texture) {}
int main()
{
    CIEngine_FreeTextureFromEngineResourcePools(1);
    std::printf("AUTO_TINY_00b23680_TEST PASS\n");
    return 0;
}