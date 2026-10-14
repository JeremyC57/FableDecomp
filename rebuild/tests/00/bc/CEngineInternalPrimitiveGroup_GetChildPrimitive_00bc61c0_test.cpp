#include <cstdio>
struct CPrim { virtual void PostRender() { ++posts; } virtual CPrim* GetAsOcclusionPrimitive() { return occ; } virtual void ResetOcclusionInfo(long l) { layer = l; } virtual void SetAnimation(void* a) { arg = a; } virtual void SetPosition(void* a) { arg = a; }
    unsigned long RenderLayerMask; unsigned char PostRenderEnable; int posts; CPrim* occ; long layer; void* arg; CPrim() : RenderLayerMask(0), PostRenderEnable(0), posts(0), occ(0), layer(0), arg(0) { } };
struct CEnginePrimitiveHandle { long m_id; CPrim* m_pPrimitive; };
static int g_erased = 0; static unsigned long g_updFlags = 0; static int g_upd = 0;
struct CRetailHandleVector { CEnginePrimitiveHandle* m_pFirst; CEnginePrimitiveHandle* m_pLast; CEnginePrimitiveHandle* m_pEnd; unsigned long GetCount() const { return m_pLast - m_pFirst; }
    void erase(CEnginePrimitiveHandle* f, CEnginePrimitiveHandle* l) { ++g_erased; m_pLast = f; } void clear() { erase(m_pFirst, m_pLast); } };
static void UpdatePosition(void*, unsigned long f) { ++g_upd; g_updFlags = f; }
struct CGroup { CRetailHandleVector Primitives; CPrim* GetChildPrimitive(unsigned long i) { if (i < Primitives.GetCount() && Primitives.m_pFirst[i].m_pPrimitive != 0) return Primitives.m_pFirst[i].m_pPrimitive; return 0; } };
int main() { CPrim a; CEnginePrimitiveHandle hs[2] = { { 0, &a }, { 1, 0 } }; CGroup g; g.Primitives.m_pFirst = hs; g.Primitives.m_pLast = hs + 2; if (g.GetChildPrimitive(0) != &a || g.GetChildPrimitive(1) != 0 || g.GetChildPrimitive(2) != 0 || g.GetChildPrimitive((unsigned long)-1) != 0) { std::printf("00bc61c0_TEST FAIL\n"); return 1; } std::printf("00bc61c0_TEST PASS\n"); return 0; }