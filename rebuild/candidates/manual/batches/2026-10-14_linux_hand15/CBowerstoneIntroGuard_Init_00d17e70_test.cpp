#include <stdio.h>
#define SCRIPT_SLOT(n) virtual void slot##n() {}
#define SCRIPT_SLOTS10(d) SCRIPT_SLOT(d##0) SCRIPT_SLOT(d##1) SCRIPT_SLOT(d##2) SCRIPT_SLOT(d##3) SCRIPT_SLOT(d##4) SCRIPT_SLOT(d##5) SCRIPT_SLOT(d##6) SCRIPT_SLOT(d##7) SCRIPT_SLOT(d##8) SCRIPT_SLOT(d##9)
static long g_mode = -1, g_target = -1; static int g_seq[2]; static int g_n; static bool g_doneAtSetTarget;
struct CBowerstoneIntroGuard;
static CBowerstoneIntroGuard* g_guard;
struct CScriptInterface {
    SCRIPT_SLOTS10(0) SCRIPT_SLOTS10(1) SCRIPT_SLOTS10(2) SCRIPT_SLOTS10(3) SCRIPT_SLOTS10(4) SCRIPT_SLOTS10(5) SCRIPT_SLOTS10(6) SCRIPT_SLOTS10(7) SCRIPT_SLOTS10(8) SCRIPT_SLOTS10(9)
    SCRIPT_SLOTS10(10) SCRIPT_SLOTS10(11) SCRIPT_SLOTS10(12) SCRIPT_SLOTS10(13) SCRIPT_SLOTS10(14) SCRIPT_SLOTS10(15) SCRIPT_SLOTS10(16) SCRIPT_SLOTS10(17) SCRIPT_SLOTS10(18) SCRIPT_SLOTS10(19)
    SCRIPT_SLOTS10(20) SCRIPT_SLOTS10(21) SCRIPT_SLOTS10(22) SCRIPT_SLOTS10(23) SCRIPT_SLOTS10(24) SCRIPT_SLOTS10(25) SCRIPT_SLOTS10(26) SCRIPT_SLOTS10(27) SCRIPT_SLOTS10(28) SCRIPT_SLOTS10(29)
    SCRIPT_SLOTS10(30) SCRIPT_SLOTS10(31) SCRIPT_SLOT(320)
    virtual void SetMode(long mode);
    SCRIPT_SLOT(322) SCRIPT_SLOT(323) SCRIPT_SLOT(324) SCRIPT_SLOT(325) SCRIPT_SLOT(326) SCRIPT_SLOT(327) SCRIPT_SLOT(328) SCRIPT_SLOT(329)
    SCRIPT_SLOT(330) SCRIPT_SLOT(331) SCRIPT_SLOT(332) SCRIPT_SLOT(333) SCRIPT_SLOT(334) SCRIPT_SLOT(335) SCRIPT_SLOT(336) SCRIPT_SLOT(337)
    virtual void SetTarget(long thing);
};
struct CBowerstoneIntroGuard {
    unsigned char _pad0[0x40]; CScriptInterface* m_pScript; unsigned char _pad1[0x38]; long m_target; unsigned char _pad2[4]; bool m_done;
    void Init();
};
void CScriptInterface::SetMode(long mode) { g_mode = mode; g_seq[g_n++] = 1; }
void CScriptInterface::SetTarget(long thing) { g_target = thing; g_seq[g_n++] = 2; g_doneAtSetTarget = g_guard->m_done; }
void CBowerstoneIntroGuard::Init() { m_pScript->SetMode(0); m_done = false; m_pScript->SetTarget(m_target); }
int main()
{
    CScriptInterface script; CBowerstoneIntroGuard g; g.m_pScript = &script; g.m_target = 1234; g.m_done = true; g_guard = &g;
    g.Init();
    bool ok = g_n == 2 && g_seq[0] == 1 && g_seq[1] == 2 && g_mode == 0 && g_target == 1234 && !g.m_done && !g_doneAtSetTarget;
    if (!ok) { printf("00d17e70_TEST FAIL\n"); return 1; }
    printf("00d17e70_TEST PASS\n"); return 0;
}
