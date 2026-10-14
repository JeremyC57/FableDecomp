#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
struct CPatch { virtual void v000() { } virtual void v001() { } virtual void v002() { } virtual void v003() { } virtual void v004() { } virtual void Tesselate() { std::strcat(g_seq, "T"); } };
struct CTreeNode { unsigned char _pad[0xa0]; CPatch* m_pPatch; unsigned char _pada4[0x3f]; bool m_tesselated; unsigned char _pade4[4]; void UpdateTesselation() { if (!m_tesselated) { if (m_pPatch) m_pPatch->Tesselate(); m_tesselated = true; } } };
struct CBranch { virtual void v00() { } CTreeNode m_nodeA; CTreeNode m_nodeB; virtual void UpdateTesselation() { m_nodeA.UpdateTesselation(); m_nodeB.UpdateTesselation(); } };

int main() {
    CBranch b; CPatch p; std::memset(&b.m_nodeA, 0, sizeof b.m_nodeA); std::memset(&b.m_nodeB, 0, sizeof b.m_nodeB); b.m_nodeA.m_pPatch = &p; b.UpdateTesselation(); if (std::strcmp(g_seq, "T") || !b.m_nodeA.m_tesselated || !b.m_nodeB.m_tesselated) { std::printf("00bf6c80_TEST FAIL\n"); return 1; } b.UpdateTesselation(); if (std::strcmp(g_seq, "T")) { std::printf("00bf6c80_TEST FAIL\n"); return 1; }
    std::printf("00bf6c80_TEST PASS\n"); return 0;
}
