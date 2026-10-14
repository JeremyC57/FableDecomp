#include <cstdio>
struct CPrim { virtual void PostRender() { ++posts; } virtual CPrim* GetAsOcclusionPrimitive() { return occ; } virtual void ResetOcclusionInfo(long l) { layer = l; } virtual void SetAnimation(void* a) { arg = a; } virtual void SetPosition(void* a) { arg = a; }
    unsigned long RenderLayerMask; unsigned char PostRenderEnable; int posts; CPrim* occ; long layer; void* arg; CPrim() : RenderLayerMask(0), PostRenderEnable(0), posts(0), occ(0), layer(0), arg(0) { } };
struct CEnginePrimitiveHandle { long m_id; CPrim* m_pPrimitive; };
static int g_erased = 0; static unsigned long g_updFlags = 0; static int g_upd = 0;
struct CRetailHandleVector { CEnginePrimitiveHandle* m_pFirst; CEnginePrimitiveHandle* m_pLast; CEnginePrimitiveHandle* m_pEnd; unsigned long GetCount() const { return m_pLast - m_pFirst; }
    void erase(CEnginePrimitiveHandle* f, CEnginePrimitiveHandle* l) { ++g_erased; m_pLast = f; } void clear() { erase(m_pFirst, m_pLast); } };
static void UpdatePosition(void*, unsigned long f) { ++g_upd; g_updFlags = f; }
struct CChanger { unsigned long RenderLayerMask; CRetailHandleVector Primitives; long m_currentIndex; unsigned long m_flags;
    unsigned long Compute() { unsigned long mask = 0; long i = m_currentIndex; if (i != -1 && (unsigned long)i < Primitives.GetCount()) { CPrim* c = Primitives.m_pFirst[i].m_pPrimitive; if (c) mask = c->RenderLayerMask; } return mask; }
    void ClearAllChildPrimitives() { Primitives.clear(); unsigned long mask = Compute(); if (RenderLayerMask != mask) { RenderLayerMask = mask; UpdatePosition(this, m_flags); } } };
int main() { CPrim a; a.RenderLayerMask = 3; CEnginePrimitiveHandle hs[1] = { { 0, &a } }; CChanger c; c.RenderLayerMask = 3; c.m_currentIndex = 0; c.m_flags = 0x55; c.Primitives.m_pFirst = hs; c.Primitives.m_pLast = hs + 1;
    c.ClearAllChildPrimitives(); if (g_erased != 1 || c.RenderLayerMask != 0 || g_upd != 1 || g_updFlags != 0x55) { std::printf("00bc5c00_TEST FAIL\n"); return 1; } c.ClearAllChildPrimitives(); if (g_upd != 1) { std::printf("00bc5c00_TEST FAIL\n"); return 1; } std::printf("00bc5c00_TEST PASS\n"); return 0; }