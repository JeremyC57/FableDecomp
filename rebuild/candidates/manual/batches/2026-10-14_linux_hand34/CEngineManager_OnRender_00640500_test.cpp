#include <cstdio>
static int g_upd = 0, g_paused = 0, g_added = 0, g_rendered = 0; static void* g_ctx = 0; static long g_flags = -1;
struct CEngineComponent_00640320 { bool m_active; void* m_lastView; CEngineComponent_00640320(bool a) : m_active(a), m_lastView(0) { }
    virtual void OnUpdate(void* c) { ++g_upd; g_ctx = c; } virtual void OnPausedUpdate() { ++g_paused; } virtual void AddMap(void*, long f) { ++g_added; g_flags = f; }
    virtual void OnRender(void* v) { ++g_rendered; m_lastView = v; } virtual bool IsActive() { return m_active; } };
struct SComponentNode { SComponentNode* m_pNext; SComponentNode* m_pPrev; CEngineComponent_00640320* m_pComponent; };
static SComponentNode g_head, g_n1, g_n2; static CEngineComponent_00640320 g_c1(true), g_c2(false);
static void Link() { g_head.m_pNext = &g_n1; g_n1.m_pNext = &g_n2; g_n2.m_pNext = &g_head; g_n1.m_pComponent = &g_c1; g_n2.m_pComponent = &g_c2; g_head.m_pComponent = 0; }
struct SViewEntry { unsigned char _body[0x10]; }; static int g_erased = 0;
struct CRetailViewVector { SViewEntry* m_pFirst; SViewEntry* m_pLast; SViewEntry* m_pEnd; void erase(SViewEntry* f, SViewEntry*) { ++g_erased; m_pLast = f; } };
struct CEngineManager { unsigned char _pad[4]; SComponentNode* m_pComponents; CRetailViewVector m_views;
    void OnRender() { for (SComponentNode* n = m_pComponents->m_pNext; n != m_pComponents; n = n->m_pNext) { if (n->m_pComponent->IsActive()) { for (SViewEntry* v = m_views.m_pFirst; v != m_views.m_pLast; ++v) n->m_pComponent->OnRender(v); } } m_views.erase(m_views.m_pFirst, m_views.m_pLast); } };
int main() {
    Link(); CEngineManager m; m.m_pComponents = &g_head; SViewEntry views[2]; m.m_views.m_pFirst = views; m.m_views.m_pLast = views + 2; m.m_views.m_pEnd = views + 2;
    m.OnRender(); if (g_rendered != 2 || g_c1.m_lastView != views + 1 || g_erased != 1 || m.m_views.m_pLast != views) { std::printf("00640500_TEST FAIL\n"); return 1; }
    m.OnRender(); if (g_rendered != 2 || g_erased != 2) { std::printf("00640500_TEST FAIL\n"); return 1; }
    std::printf("00640500_TEST PASS\n"); return 0;
}
