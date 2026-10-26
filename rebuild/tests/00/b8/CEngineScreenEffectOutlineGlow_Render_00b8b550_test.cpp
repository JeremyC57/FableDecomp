#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[8] = ""; static int g_n = 0;
class CEngineComponentRenderParams { public: long m_a; long m_kind; };
class CEngineScreenEffectOutlineGlow { public: unsigned char _pad[8]; bool m_enabled; void ApplyOutline() { g_seq[g_n++] = 'A'; } void ReleaseTextures() { g_seq[g_n++] = 'R'; }
    void Render(const CEngineComponentRenderParams& p) { if (m_enabled && p.m_kind == 0x4000000) { ApplyOutline(); ReleaseTextures(); } } };
int main() {
    CEngineScreenEffectOutlineGlow g; CEngineComponentRenderParams p = { 0, 0x4000000 }; g.m_enabled = false; g.Render(p); if (g_seq[0]) { std::printf("00b8b550_TEST FAIL\n"); return 1; }
    g.m_enabled = true; p.m_kind = 1; g.Render(p); if (g_seq[0]) { std::printf("00b8b550_TEST FAIL\n"); return 1; }
    p.m_kind = 0x4000000; g.Render(p); if (std::strcmp(g_seq, "AR")) { std::printf("00b8b550_TEST FAIL\n"); return 1; }
    std::printf("00b8b550_TEST PASS\n"); return 0;
}