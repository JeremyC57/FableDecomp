#include <cstdio>
struct CPrim { virtual void PostRender() { ++posts; } virtual CPrim* GetAsOcclusionPrimitive() { return occ; } virtual void ResetOcclusionInfo(long l) { layer = l; } virtual void SetAnimation(void* a) { arg = a; } virtual void SetPosition(void* a) { arg = a; }
    unsigned long RenderLayerMask; unsigned char PostRenderEnable; int posts; CPrim* occ; long layer; void* arg; CPrim() : RenderLayerMask(0), PostRenderEnable(0), posts(0), occ(0), layer(0), arg(0) { } };
struct CEnginePrimitiveHandle { long m_id; CPrim* m_pPrimitive; };
static int g_erased = 0; static unsigned long g_updFlags = 0; static int g_upd = 0;
struct CRetailHandleVector { CEnginePrimitiveHandle* m_pFirst; CEnginePrimitiveHandle* m_pLast; CEnginePrimitiveHandle* m_pEnd; unsigned long GetCount() const { return m_pLast - m_pFirst; }
    void erase(CEnginePrimitiveHandle* f, CEnginePrimitiveHandle* l) { ++g_erased; m_pLast = f; } void clear() { erase(m_pFirst, m_pLast); } };
static void UpdatePosition(void*, unsigned long f) { ++g_upd; g_updFlags = f; }
struct CGroup { unsigned long RenderLayerMask; CRetailHandleVector Primitives; unsigned long PrimitiveFlags;
    void Update() { unsigned long mask = 0; for (CEnginePrimitiveHandle* h = Primitives.m_pFirst; h != Primitives.m_pLast; ++h) if (h->m_pPrimitive) mask |= h->m_pPrimitive->RenderLayerMask; if (RenderLayerMask != mask) { RenderLayerMask = mask; UpdatePosition(this, PrimitiveFlags); } }
    void ClearAllChildPrimitives() { Primitives.clear(); Update(); } };
int main() { CPrim a; a.RenderLayerMask = 1; CEnginePrimitiveHandle hs[1] = { { 0, &a } }; CGroup g; g.RenderLayerMask = 1; g.PrimitiveFlags = 9; g.Primitives.m_pFirst = hs; g.Primitives.m_pLast = hs + 1;
    g.ClearAllChildPrimitives(); if (g_erased != 1 || g.RenderLayerMask != 0 || g_upd != 1 || g_updFlags != 9) { std::printf("00bc6580_TEST FAIL\n"); return 1; } std::printf("00bc6580_TEST PASS\n"); return 0; }
