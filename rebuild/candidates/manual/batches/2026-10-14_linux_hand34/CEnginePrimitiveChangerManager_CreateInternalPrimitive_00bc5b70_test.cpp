#include <cstdio>
static int g_baseType = -1; static unsigned long g_baseFlags = 0;
struct CEngineInternalPrimitiveBase { virtual void v00() { } unsigned char _pad04[0x30]; unsigned char PostRenderEnable; unsigned char _pad35[0x13];
    CEngineInternalPrimitiveBase(long type, unsigned long flags) { g_baseType = type; g_baseFlags = flags; PostRenderEnable = 0; } };
struct CEnginePrimitiveHandle { long m_id; CEngineInternalPrimitiveBase* m_pPrimitive; };
struct CRetailHandleVector { CEnginePrimitiveHandle* m_pFirst; CEnginePrimitiveHandle* m_pLast; CEnginePrimitiveHandle* m_pEnd; long m_reserved; CRetailHandleVector() : m_pFirst(0), m_pLast(0), m_pEnd(0), m_reserved(0) { } };
struct CEngineInternalPrimitiveChanger : CEngineInternalPrimitiveBase { CRetailHandleVector Primitives; long m_currentIndex; long _pad5c; unsigned long m_flags;
    CEngineInternalPrimitiveChanger() : CEngineInternalPrimitiveBase(2, 0), Primitives(), m_flags(0) { PostRenderEnable |= 8; m_currentIndex = -1; } };
struct CEnginePrimitiveChangerManager { CEngineInternalPrimitiveBase* CreateInternalPrimitive(long) { return new CEngineInternalPrimitiveChanger; } };
int main() {
    CEnginePrimitiveChangerManager m; CEngineInternalPrimitiveChanger* c = (CEngineInternalPrimitiveChanger*)m.CreateInternalPrimitive(0);
    if (!c || g_baseType != 2 || g_baseFlags != 0 || c->Primitives.m_pFirst || c->Primitives.m_pLast || c->Primitives.m_pEnd || c->Primitives.m_reserved || c->m_currentIndex != -1 || c->m_flags != 0 || !(c->PostRenderEnable & 8)) { std::printf("00bc5b70_TEST FAIL\n"); return 1; }
    delete c;
    std::printf("00bc5b70_TEST PASS\n"); return 0;
}
