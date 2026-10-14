#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[8] = ""; static int g_n = 0;
struct CRenderTarget { unsigned char _body[0x24]; void Uninitialise() { g_seq[g_n++] = 'U'; } };
struct CShaderHolder_009f2e20 { unsigned char _body[0x10]; void Release() { g_seq[g_n++] = 'S'; } };
struct CBufferHolder_009f9f70 { unsigned char _body[0xc]; void Release() { g_seq[g_n++] = 'B'; } };
struct CEngineShadowRenderer { unsigned char _pad[0x2af]; bool m_defaultPoolHeld; unsigned char _pad2b0[0x14]; CRenderTarget m_renderTarget; CBufferHolder_009f9f70 m_bufferA; unsigned char _pad2f4[4]; CShaderHolder_009f2e20 m_shaderA; CShaderHolder_009f2e20 m_shaderB; CBufferHolder_009f9f70 m_bufferB;
    void OnReleaseDefaultPoolResources() { if (m_defaultPoolHeld) { m_renderTarget.Uninitialise(); m_shaderB.Release(); m_shaderA.Release(); m_bufferB.Release(); m_bufferA.Release(); m_defaultPoolHeld = false; } } };
int main() {
    CEngineShadowRenderer* r = (CEngineShadowRenderer*)new char[sizeof(CEngineShadowRenderer)]; r->m_defaultPoolHeld = false; r->OnReleaseDefaultPoolResources(); if (g_seq[0]) { std::printf("00b4b830_TEST FAIL\n"); return 1; }
    r->m_defaultPoolHeld = true; r->OnReleaseDefaultPoolResources(); if (std::strcmp(g_seq, "USSBB") || r->m_defaultPoolHeld) { std::printf("00b4b830_TEST FAIL\n"); return 1; }
    delete[] (char*)r;
    std::printf("00b4b830_TEST PASS\n"); return 0;
}
