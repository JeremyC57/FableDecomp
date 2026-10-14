#include <cstdio>
#include <new>
static int g_baseCtors = 0;
struct CBaseClass { virtual void v00() { } CBaseClass() { ++g_baseCtors; } };
struct CIVCountedPointer { void* Object; CIVCountedPointer() : Object(0) { } };
struct C3DAnimationBlendState : CBaseClass { CIVCountedPointer Data; C3DAnimationBlendState() { } };
struct C3DAnimationInterpolator : CBaseClass { C3DAnimationBlendState StartState; C3DAnimationBlendState EndState; C3DAnimationInterpolator() { } };
int main()
{
    unsigned char raw[sizeof(C3DAnimationInterpolator)]; for (int k = 0; k < (int)sizeof(raw); ++k) raw[k] = 0xAA;
    C3DAnimationInterpolator* p = new (raw) C3DAnimationInterpolator;
    if (g_baseCtors != 3 || sizeof(C3DAnimationInterpolator) != 0x14 || (char*)&p->StartState != (char*)p + 4 || (char*)&p->EndState != (char*)p + 0xc || p->StartState.Data.Object != 0 || p->EndState.Data.Object != 0) { std::printf("004c2e20_TEST FAIL\n"); return 1; }
    std::printf("004c2e20_TEST PASS\n"); return 0;
}
