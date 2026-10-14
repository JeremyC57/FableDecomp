#include <cstdio>
// Model of CEngineInternalAttachedPrimitiveInfo::CEngineInternalAttachedPrimitiveInfo @ 0x00B3AAE0.
static int g_baseCalls;
struct CBaseClass { virtual ~CBaseClass() {} CBaseClass() { ++g_baseCalls; } };
struct CEnginePrimitiveHandle : CBaseClass { void* m_pPrimitive; CEnginePrimitiveHandle() : CBaseClass(), m_pPrimitive(0) {} };
struct Info : CBaseClass { CEnginePrimitiveHandle Handle; Info() : CBaseClass(), Handle() {} };
int main()
{
    Info i;
    if (g_baseCalls != 2 || i.Handle.m_pPrimitive != 0) { std::printf("00b3aae0_TEST FAIL\n"); return 1; }
    std::printf("00b3aae0_TEST PASS\n"); return 0;
}