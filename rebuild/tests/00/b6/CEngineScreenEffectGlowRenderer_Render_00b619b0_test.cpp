#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
static std::string g_seq;
class CEngineScreenEffectGlowRenderer { public: unsigned char _pad[8]; bool m_enabled; unsigned char _pad09[3]; bool m_colourDirty; unsigned char _pad0d[0x20b]; void* m_pGlowTexture;
    void ComputeGlowColor() { g_seq += 'G'; } void CaptureBackBuffer() { g_seq += 'C'; } void ApplyGlow() { g_seq += 'A'; } void ShowGlowTexture() { g_seq += 'S'; } void FinishRender() { g_seq += 'F'; }
    void Render(const void*) { if (m_enabled) { if (m_colourDirty) ComputeGlowColor(); CaptureBackBuffer(); ApplyGlow(); if (m_pGlowTexture) ShowGlowTexture(); FinishRender(); } } };
int main() {
    CEngineScreenEffectGlowRenderer* r = (CEngineScreenEffectGlowRenderer*)new char[sizeof(CEngineScreenEffectGlowRenderer)]; r->m_enabled = false; r->Render(0); if (!g_seq.empty()) { std::printf("00b619b0_TEST FAIL\n"); return 1; }
    r->m_enabled = true; r->m_colourDirty = false; r->m_pGlowTexture = 0; r->Render(0); if (g_seq != "CAF") { std::printf("00b619b0_TEST FAIL\n"); return 1; }
    g_seq = ""; r->m_colourDirty = true; r->m_pGlowTexture = r; r->Render(0); if (g_seq != "GCASF") { std::printf("00b619b0_TEST FAIL\n"); return 1; }
    delete[] (char*)r;
    std::printf("00b619b0_TEST PASS\n"); return 0;
}