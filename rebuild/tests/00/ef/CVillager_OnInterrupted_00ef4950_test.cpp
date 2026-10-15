#include <cstdio>
#include <cstring>
struct CScriptThing;
static const CScriptThing* g_stopped; static int g_stopCalls;
struct CGameScriptInterface { virtual void OnScriptThingStopped(const CScriptThing* thing) { g_stopped = thing; ++g_stopCalls; } };
struct CScriptThing {
    long m_handle; long m_carried; bool m_valid;
    virtual long GetDataHandle() const { return m_handle; }
    virtual bool IsCarrying(long handle) const { return handle == m_carried; }
    virtual bool IsValid() const { return m_valid; }
};
struct CVillager {
    long m_0; CGameScriptInterface* m_pInterface; CScriptThing Me; CScriptThing Crate; bool m_paused;
    bool IsScriptPaused() { return m_paused; }
    void OnInterrupted()
    {
        if (Crate.IsValid()) {
            if (Crate.IsCarrying(Me.GetDataHandle())) {
                if (!IsScriptPaused())
                    m_pInterface->OnScriptThingStopped(&Me);
            }
        }
    }
};
int main()
{
    int ok = 1;
    CGameScriptInterface gi; CVillager v; v.m_0 = 0; v.m_pInterface = &gi; v.m_paused = false;
    v.Me.m_handle = 5; v.Me.m_carried = 0; v.Me.m_valid = true;
    v.Crate.m_handle = 9; v.Crate.m_carried = 5; v.Crate.m_valid = false;
    v.OnInterrupted();
    if (g_stopCalls != 0) ok = 0;
    v.Crate.m_valid = true; v.m_paused = true; v.OnInterrupted();
    if (g_stopCalls != 0) ok = 0;
    v.Crate.m_carried = 6; v.m_paused = false; v.OnInterrupted();
    if (g_stopCalls != 0) ok = 0;
    v.Crate.m_carried = 5; v.OnInterrupted();
    if (g_stopCalls != 1 || g_stopped != &v.Me) ok = 0;
    if (!ok) { std::printf("00ef4950_TEST FAIL\n"); return 1; }
    std::printf("00ef4950_TEST PASS\n");
    return 0;
}