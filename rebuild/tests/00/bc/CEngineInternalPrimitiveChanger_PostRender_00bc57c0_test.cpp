#include <cstdio>
struct CPrim { virtual void PostRender() { ++posts; } virtual CPrim* GetAsOcclusionPrimitive() { return occ; } virtual void ResetOcclusionInfo(long l) { layer = l; } virtual void SetAnimation(void* a) { arg = a; } virtual void SetPosition(void* a) { arg = a; }
    unsigned long RenderLayerMask; unsigned char PostRenderEnable; int posts; CPrim* occ; long layer; void* arg; CPrim() : RenderLayerMask(0), PostRenderEnable(0), posts(0), occ(0), layer(0), arg(0) { } };
struct CEnginePrimitiveHandle { long m_id; CPrim* m_pPrimitive; };
static int g_erased = 0; static unsigned long g_updFlags = 0; static int g_upd = 0;
struct CRetailHandleVector { CEnginePrimitiveHandle* m_pFirst; CEnginePrimitiveHandle* m_pLast; CEnginePrimitiveHandle* m_pEnd; unsigned long GetCount() const { return m_pLast - m_pFirst; }
    void erase(CEnginePrimitiveHandle* f, CEnginePrimitiveHandle* l) { ++g_erased; m_pLast = f; } void clear() { erase(m_pFirst, m_pLast); } };
static void UpdatePosition(void*, unsigned long f) { ++g_upd; g_updFlags = f; }
struct CChanger { CRetailHandleVector Primitives; long m_currentIndex; void PostRender() { long i = m_currentIndex; if (i != -1 && (unsigned long)i < Primitives.GetCount()) { CPrim* c = Primitives.m_pFirst[i].m_pPrimitive; if (c && (c->PostRenderEnable & 8)) c->PostRender(); } } };
int main() { CPrim a; a.PostRenderEnable = 8; CEnginePrimitiveHandle hs[2] = { { 0, &a }, { 1, 0 } }; CChanger c; c.Primitives.m_pFirst = hs; c.Primitives.m_pLast = hs + 2; c.m_currentIndex = -1; c.PostRender(); if (a.posts) { std::printf("00bc57c0_TEST FAIL\n"); return 1; }
    c.m_currentIndex = 0; c.PostRender(); if (a.posts != 1) { std::printf("00bc57c0_TEST FAIL\n"); return 1; } c.m_currentIndex = 1; c.PostRender(); c.m_currentIndex = 5; c.PostRender(); a.PostRenderEnable = 0; c.m_currentIndex = 0; c.PostRender(); if (a.posts != 1) { std::printf("00bc57c0_TEST FAIL\n"); return 1; } std::printf("00bc57c0_TEST PASS\n"); return 0; }