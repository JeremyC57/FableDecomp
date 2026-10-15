#include <cstdio>
#include <cstring>
static void* g_sinkTarget; static int g_refreshes;
struct CTargetSink { virtual void SetTarget(void* target) { g_sinkTarget = target; } };
struct CTargetView { virtual void Refresh() { ++g_refreshes; } void* m_target; };
struct CRenderTargetOwner {
    CTargetView* m_pView; void* m_pActive; CTargetSink* m_pSink;
    void SetTarget(void* target)
    {
        if (m_pSink) m_pSink->SetTarget(target);
        if (m_pActive) { CTargetView* view = m_pView; view->m_target = target; view->Refresh(); }
    }
};
int main()
{
    int ok = 1;
    CTargetSink sink; CTargetView view; view.m_target = 0; int t1, t2;
    CRenderTargetOwner o; o.m_pView = &view; o.m_pActive = 0; o.m_pSink = &sink;
    o.SetTarget(&t1);
    if (g_sinkTarget != &t1 || view.m_target != 0 || g_refreshes != 0) ok = 0;
    o.m_pActive = &o; o.m_pSink = 0;
    o.SetTarget(&t2);
    if (g_sinkTarget != &t1 || view.m_target != &t2 || g_refreshes != 1) ok = 0;
    if (!ok) { std::printf("00c336b0_TEST FAIL\n"); return 1; }
    std::printf("00c336b0_TEST PASS\n");
    return 0;
}
