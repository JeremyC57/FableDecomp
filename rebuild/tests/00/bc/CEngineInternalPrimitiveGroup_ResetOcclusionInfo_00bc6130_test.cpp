#include <cstdio>
struct CPrim { virtual void PostRender() { ++posts; } virtual CPrim* GetAsOcclusionPrimitive() { return occ; } virtual void ResetOcclusionInfo(long l) { layer = l; } virtual void SetAnimation(void* a) { arg = a; } virtual void SetPosition(void* a) { arg = a; }
    unsigned long RenderLayerMask; unsigned char PostRenderEnable; int posts; CPrim* occ; long layer; void* arg; CPrim() : RenderLayerMask(0), PostRenderEnable(0), posts(0), occ(0), layer(0), arg(0) { } };
struct CEnginePrimitiveHandle { long m_id; CPrim* m_pPrimitive; };
static int g_erased = 0; static unsigned long g_updFlags = 0; static int g_upd = 0;
struct CRetailHandleVector { CEnginePrimitiveHandle* m_pFirst; CEnginePrimitiveHandle* m_pLast; CEnginePrimitiveHandle* m_pEnd; unsigned long GetCount() const { return m_pLast - m_pFirst; }
    void erase(CEnginePrimitiveHandle* f, CEnginePrimitiveHandle* l) { ++g_erased; m_pLast = f; } void clear() { erase(m_pFirst, m_pLast); } };
static void UpdatePosition(void*, unsigned long f) { ++g_upd; g_updFlags = f; }
static long g_baseLayer = -5;
struct CGroup { CRetailHandleVector Primitives; void ResetOcclusionInfoBase(long l) { g_baseLayer = l; }
    void ResetOcclusionInfo(long layer) { if (layer == -1) return; ResetOcclusionInfoBase(layer); for (CEnginePrimitiveHandle* h = Primitives.m_pFirst; h != Primitives.m_pLast; ++h) { CPrim* c = h->m_pPrimitive; if (c) { CPrim* o = c->GetAsOcclusionPrimitive(); if (o) o->ResetOcclusionInfo(layer); } } } };
int main() { CPrim a, occ, b; a.occ = &occ; CEnginePrimitiveHandle hs[3] = { { 0, &a }, { 1, 0 }, { 2, &b } }; CGroup g; g.Primitives.m_pFirst = hs; g.Primitives.m_pLast = hs + 3; g.ResetOcclusionInfo(-1); if (g_baseLayer != -5) { std::printf("00bc6130_TEST FAIL\n"); return 1; }
    g.ResetOcclusionInfo(2); if (g_baseLayer != 2 || occ.layer != 2 || b.layer != 0) { std::printf("00bc6130_TEST FAIL\n"); return 1; } std::printf("00bc6130_TEST PASS\n"); return 0; }