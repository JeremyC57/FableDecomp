#include <cstdio>

class CMovableResourceMemoryPool;
class CIEngine;

CMovableResourceMemoryPool* __fastcall CIEngine_GetAnimationMemoryPool(CIEngine* engine)
{
    return 0;
}

int main()
{
    CMovableResourceMemoryPool* r = CIEngine_GetAnimationMemoryPool((CIEngine*)0x1234);
    if (r == 0) {
        std::printf("CIEngine_00b236a0_TEST PASS\n");
        return 0;
    }
    std::printf("CIEngine_00b236a0_TEST FAIL\n");
    return 1;
}