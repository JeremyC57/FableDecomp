#include <cstdio>
static int g_upd = 0, g_paused = 0, g_added = 0, g_rendered = 0; static void* g_ctx = 0; static long g_flags = -1;
struct CEngineComponent_00640320 { bool m_active; void* m_lastView; CEngineComponent_00640320(bool a) : m_active(a), m_lastView(0) { }
    virtual void OnUpdate(void* c) { ++g_upd; g_ctx = c; } virtual void OnPausedUpdate() { ++g_paused; } virtual void AddMap(void*, long f) { ++g_added; g_flags = f; }
    virtual void OnRender(void* v) { ++g_rendered; m_lastView = v; } virtual bool IsActive() { return m_active; } };
struct SComponentNode { SComponentNode* m_pNext; SComponentNode* m_pPrev; CEngineComponent_00640320* m_pComponent; };
static SComponentNode g_head, g_n1, g_n2; static CEngineComponent_00640320 g_c1(true), g_c2(false);
static void Link() { g_head.m_pNext = &g_n1; g_n1.m_pNext = &g_n2; g_n2.m_pNext = &g_head; g_n1.m_pComponent = &g_c1; g_n2.m_pComponent = &g_c2; g_head.m_pComponent = 0; }
struct CEngineManager { unsigned char _pad[4]; SComponentNode* m_pComponents;
    void OnPausedUpdate() { for (SComponentNode* n = m_pComponents->m_pNext; n != m_pComponents; n = n->m_pNext) { CEngineComponent_00640320* c = n->m_pComponent; if (c->IsActive()) c->OnPausedUpdate(); } } };
int main() {
    Link(); CEngineManager m; m.m_pComponents = &g_head; m.OnPausedUpdate(); if (g_paused != 1) { std::printf("00640360_TEST FAIL\n"); return 1; }
    g_c2.m_active = true; m.OnPausedUpdate(); if (g_paused != 3) { std::printf("00640360_TEST FAIL\n"); return 1; }
    std::printf("00640360_TEST PASS\n"); return 0;
}