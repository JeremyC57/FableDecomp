#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
static void* g_reset = 0; static int ResetEvent(void* e) { g_reset = e; return 1; } struct CControl { virtual long v0() { return 0; } virtual long v1() { return 0; } virtual long v2() { return 0; } virtual long v3() { return 0; } virtual long v4() { return 0; } virtual long Pause() { std::strcat(g_seq, "P"); return 77; } };
class CPlayer { public: unsigned char _pad[0x98]; CControl* m_pControl; unsigned char _p[0x34]; void* m_hEvent; void* m_hThread; long Pause() { if (!m_hEvent || !m_hThread) return 0x80004005; ResetEvent(m_hEvent); if (!m_pControl) return 0x8004020a; return m_pControl->Pause(); } };

int main() {
    CControl c; CPlayer* p = new CPlayer; std::memset(p, 0, sizeof *p); if (p->Pause() != (long)0x80004005) { std::printf("00ca9a90_TEST FAIL\n"); return 1; } int e, t; p->m_hEvent = &e; p->m_hThread = &t; if (p->Pause() != (long)0x8004020a || g_reset != &e) { std::printf("00ca9a90_TEST FAIL\n"); return 1; } p->m_pControl = &c; if (p->Pause() != 77 || std::strcmp(g_seq, "P")) { std::printf("00ca9a90_TEST FAIL\n"); return 1; } delete p;
    std::printf("00ca9a90_TEST PASS\n"); return 0;
}