#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
struct SEdge { unsigned char _pad[0x28]; bool m_done; }; struct SPatch { unsigned char _pad[0x1c]; SEdge* m_edges[4]; }; static long g_failAt = -1; static void* g_ctx = 0;
struct CTess { unsigned char _pad[0x10]; SPatch* m_pPatch; bool TesselateEdgeGeneric(long i, SEdge* e, void* c) { char s[2] = { (char)('0' + i), 0 }; std::strcat(g_seq, s); g_ctx = c; (void)e; return i != g_failAt; }
    bool TesselatePatch(SPatch* patch, void* c) { m_pPatch = patch; for (long i = 0; i < 4; ++i) { SEdge* e = m_pPatch->m_edges[i]; if (e && !e->m_done) { if (!TesselateEdgeGeneric(i, e, c)) return false; } } return true; } };

int main() {
    CTess t; SPatch p; SEdge e[4]; for (int i = 0; i < 4; ++i) { e[i].m_done = false; p.m_edges[i] = &e[i]; } p.m_edges[1] = 0; e[2].m_done = true; g_failAt = -1;
    if (!t.TesselatePatch(&p, (void*)7) || t.m_pPatch != &p || std::strcmp(g_seq, "03") || g_ctx != (void*)7) { std::printf("00be3910_TEST FAIL\n"); return 1; } g_seq[0] = 0; g_failAt = 3; if (t.TesselatePatch(&p, 0) || std::strcmp(g_seq, "03")) { std::printf("00be3910_TEST FAIL\n"); return 1; } g_seq[0] = 0; g_failAt = 0; if (t.TesselatePatch(&p, 0) || std::strcmp(g_seq, "0")) { std::printf("00be3910_TEST FAIL\n"); return 1; }
    std::printf("00be3910_TEST PASS\n"); return 0;
}