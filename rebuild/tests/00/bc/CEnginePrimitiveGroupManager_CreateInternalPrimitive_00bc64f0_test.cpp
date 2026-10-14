#include <cstdio>
static int g_baseType = -1; static unsigned long g_baseFlags = 0;
struct CEngineInternalPrimitiveBase { virtual void v00() { } unsigned char _pad04[0x30]; unsigned char PostRenderEnable; unsigned char _pad35[0x13];
    CEngineInternalPrimitiveBase(long type, unsigned long flags) { g_baseType = type; g_baseFlags = flags; PostRenderEnable = 0; } };
struct CEnginePrimitiveHandle { long m_id; CEngineInternalPrimitiveBase* m_pPrimitive; };
struct CRetailHandleVector { CEnginePrimitiveHandle* m_pFirst; CEnginePrimitiveHandle* m_pLast; CEnginePrimitiveHandle* m_pEnd; CRetailHandleVector() : m_pFirst(0), m_pLast(0), m_pEnd(0) { } };
struct CEngineInternalPrimitiveGroup : CEngineInternalPrimitiveBase { long m_a; long m_b; CRetailHandleVector Primitives; unsigned long PrimitiveFlags;
    CEngineInternalPrimitiveGroup() : CEngineInternalPrimitiveBase(1, 0), m_a(0), m_b(0), Primitives(), PrimitiveFlags(0) { PostRenderEnable |= 8; } };
struct CEnginePrimitiveGroupManager { CEngineInternalPrimitiveBase* CreateInternalPrimitive(long) { return new CEngineInternalPrimitiveGroup; } };
int main() {
    CEnginePrimitiveGroupManager m; CEngineInternalPrimitiveGroup* g = (CEngineInternalPrimitiveGroup*)m.CreateInternalPrimitive(0);
    if (!g || g_baseType != 1 || g_baseFlags != 0 || g->m_a || g->m_b || g->Primitives.m_pFirst || g->Primitives.m_pLast || g->Primitives.m_pEnd || g->PrimitiveFlags || !(g->PostRenderEnable & 8)) { std::printf("00bc64f0_TEST FAIL\n"); return 1; }
    delete g;
    std::printf("00bc64f0_TEST PASS\n"); return 0;
}