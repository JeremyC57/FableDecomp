#include <cstdio>
struct CPrim { virtual void PostRender() { ++posts; } virtual CPrim* GetAsOcclusionPrimitive() { return occ; } virtual void ResetOcclusionInfo(long l) { layer = l; } virtual void SetAnimation(void* a) { arg = a; } virtual void SetPosition(void* a) { arg = a; }
    unsigned long RenderLayerMask; unsigned char PostRenderEnable; int posts; CPrim* occ; long layer; void* arg; CPrim() : RenderLayerMask(0), PostRenderEnable(0), posts(0), occ(0), layer(0), arg(0) { } };
struct CEnginePrimitiveHandle { long m_id; CPrim* m_pPrimitive; };
static int g_erased = 0; static unsigned long g_updFlags = 0; static int g_upd = 0;
struct CRetailHandleVector { CEnginePrimitiveHandle* m_pFirst; CEnginePrimitiveHandle* m_pLast; CEnginePrimitiveHandle* m_pEnd; unsigned long GetCount() const { return m_pLast - m_pFirst; }
    void erase(CEnginePrimitiveHandle* f, CEnginePrimitiveHandle* l) { ++g_erased; m_pLast = f; } void clear() { erase(m_pFirst, m_pLast); } };
static void UpdatePosition(void*, unsigned long f) { ++g_upd; g_updFlags = f; }
struct CViewport { unsigned long RenderLayerMask; CRetailHandleVector Primitives; void Update() { RenderLayerMask = 0; for (CEnginePrimitiveHandle* h = Primitives.m_pFirst; h != Primitives.m_pLast; ++h) if (h->m_pPrimitive) RenderLayerMask |= h->m_pPrimitive->RenderLayerMask; } };
int main() { CPrim a, b; a.RenderLayerMask = 1; b.RenderLayerMask = 4; CEnginePrimitiveHandle hs[3] = { { 0, &a }, { 1, 0 }, { 2, &b } }; CViewport v; v.RenderLayerMask = 99; v.Primitives.m_pFirst = hs; v.Primitives.m_pLast = hs + 3; v.Update(); if (v.RenderLayerMask != 5) { std::printf("00be1d80_TEST FAIL\n"); return 1; } std::printf("00be1d80_TEST PASS\n"); return 0; }
