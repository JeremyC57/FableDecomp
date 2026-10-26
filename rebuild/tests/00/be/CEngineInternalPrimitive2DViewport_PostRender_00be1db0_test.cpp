#include <cstdio>
struct CPrim { virtual void PostRender() { ++posts; } virtual CPrim* GetAsOcclusionPrimitive() { return occ; } virtual void ResetOcclusionInfo(long l) { layer = l; } virtual void SetAnimation(void* a) { arg = a; } virtual void SetPosition(void* a) { arg = a; }
    unsigned long RenderLayerMask; unsigned char PostRenderEnable; int posts; CPrim* occ; long layer; void* arg; CPrim() : RenderLayerMask(0), PostRenderEnable(0), posts(0), occ(0), layer(0), arg(0) { } };
class CEnginePrimitiveHandle { public: long m_id; CPrim* m_pPrimitive; };
static int g_erased = 0; static unsigned long g_updFlags = 0; static int g_upd = 0;
struct CRetailHandleVector { CEnginePrimitiveHandle* m_pFirst; CEnginePrimitiveHandle* m_pLast; CEnginePrimitiveHandle* m_pEnd; unsigned long GetCount() const { return m_pLast - m_pFirst; }
    void erase(CEnginePrimitiveHandle* f, CEnginePrimitiveHandle* l) { ++g_erased; m_pLast = f; } void clear() { erase(m_pFirst, m_pLast); } };
static void UpdatePosition(void*, unsigned long f) { ++g_upd; g_updFlags = f; }
struct CViewport { CRetailHandleVector Primitives; void PostRender() { for (CEnginePrimitiveHandle* h = Primitives.m_pFirst; h != Primitives.m_pLast; ++h) { CPrim* c = h->m_pPrimitive; if (c && (c->PostRenderEnable & 8)) c->PostRender(); } } };
int main() { CPrim a, b, c; a.PostRenderEnable = 8; b.PostRenderEnable = 0; c.PostRenderEnable = 0xff; CEnginePrimitiveHandle hs[4] = { { 0, &a }, { 1, 0 }, { 2, &b }, { 3, &c } }; CViewport g; g.Primitives.m_pFirst = hs; g.Primitives.m_pLast = hs + 4; g.PostRender(); if (a.posts != 1 || b.posts != 0 || c.posts != 1) { std::printf("00be1db0_TEST FAIL\n"); return 1; } std::printf("00be1db0_TEST PASS\n"); return 0; }