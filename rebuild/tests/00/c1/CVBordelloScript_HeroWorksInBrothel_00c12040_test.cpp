// Behaviour test for the update step at 0x00C12040 (catalog: CV_BordelloScript::HeroWorksInBrothel).
#include <cstdio>

static int g_log[8];
static int g_n = 0;

struct CRequestTimer {
    bool m_elapsed;
    void Update() { g_log[g_n++] = 1; }
    bool HasElapsed() { g_log[g_n++] = 2; return m_elapsed; }
    void Reset() { g_log[g_n++] = 3; m_elapsed = false; }
};

struct CRequestHandler {
    void* m_last;
    void Handle(void* request) { g_log[g_n++] = 4; m_last = request; }
};

struct CV_BordelloScript {
    CRequestHandler* m_pHandler;
    CRequestTimer m_timer;
    bool m_requestPending;
    unsigned char m_request[4];
    void UpdateChildren() { g_log[g_n++] = 5; }
    void HeroWorksInBrothel();
};

void CV_BordelloScript::HeroWorksInBrothel()
{
    m_timer.Update();
    UpdateChildren();
    if (m_requestPending && m_timer.HasElapsed()) {
        m_pHandler->Handle(m_request);
        m_requestPending = false;
        m_timer.Reset();
    }
}

int main()
{
    CRequestHandler h; h.m_last = 0;
    CV_BordelloScript s; s.m_pHandler = &h; s.m_timer.m_elapsed = false; s.m_requestPending = false;
    s.HeroWorksInBrothel();
    if (g_n != 2 || g_log[0] != 1 || g_log[1] != 5) { std::printf("00c12040_TEST FAIL\n"); return 1; }
    g_n = 0; s.m_requestPending = true; s.HeroWorksInBrothel();
    if (g_n != 3 || g_log[2] != 2 || !s.m_requestPending) { std::printf("00c12040_TEST FAIL\n"); return 1; }
    g_n = 0; s.m_timer.m_elapsed = true; s.HeroWorksInBrothel();
    if (g_n != 5 || g_log[3] != 4 || g_log[4] != 3 || s.m_requestPending || h.m_last != s.m_request || s.m_timer.m_elapsed) {
        std::printf("00c12040_TEST FAIL\n");
        return 1;
    }
    std::printf("00c12040_TEST PASS\n");
    return 0;
}