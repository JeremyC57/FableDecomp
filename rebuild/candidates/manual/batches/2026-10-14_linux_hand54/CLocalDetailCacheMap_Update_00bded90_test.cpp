#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
struct CFiles { void RemoveFinished() { std::strcat(g_seq, "R"); } }; struct SRec { unsigned char m_body[4]; }; static CFiles* g_list; static SRec* g_rec; static double g_time; static bool g_result; static bool g_busyDuringUpdate = false; struct CMap;
static const bool* g_pBusy = 0; struct CElement { unsigned char _body[0x58]; bool Update(CFiles* l, SRec* r, double t) { g_list = l; g_rec = r; g_time = t; g_busyDuringUpdate = *g_pBusy; std::strcat(g_seq, "U"); return g_result; } };
struct CMap { bool m_busy; CFiles* m_pFiles; long m_8; CElement m_element; SRec m_record; CMap() : m_busy(false) { g_pBusy = &m_busy; } bool Update(double t) { m_busy = true; bool u = m_element.Update(m_pFiles, &m_record, t); if (m_pFiles) m_pFiles->RemoveFinished(); m_busy = false; return u; } };

int main() {
    CMap m; CFiles f; m.m_pFiles = &f; g_result = true; if (!m.Update(2.5) || m.m_busy || g_list != &f || g_rec != &m.m_record || g_time != 2.5 || !g_busyDuringUpdate || std::strcmp(g_seq, "UR")) { std::printf("00bded90_TEST FAIL\n"); return 1; }
    g_seq[0] = 0; m.m_pFiles = 0; g_result = false; if (m.Update(1.0) || std::strcmp(g_seq, "U")) { std::printf("00bded90_TEST FAIL\n"); return 1; }
    std::printf("00bded90_TEST PASS\n"); return 0;
}
