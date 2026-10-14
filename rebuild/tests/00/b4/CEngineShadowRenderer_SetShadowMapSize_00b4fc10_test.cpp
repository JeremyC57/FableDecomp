#include <cstdio>
#include <cstring>
static const bool g_is32 = sizeof(void*) == 4 && sizeof(long) == 4;   // the layout checks hold for the 32-bit VC7.1 build
static char g_seq[32] = "";
struct CEngineShadowRenderer { unsigned char _pad[0x338]; long m_shadowMapWidth; long m_shadowMapHeight;
    void OnReleaseDefaultPoolResources() { std::strcat(g_seq, "R"); } bool InitialiseTextures() { std::strcat(g_seq, "I"); return true; }
    void SetShadowMapSize(long width, long height) { m_shadowMapWidth = width; m_shadowMapHeight = height; OnReleaseDefaultPoolResources(); InitialiseTextures(); } };
int main()
{
    CEngineShadowRenderer r; r.m_shadowMapWidth = r.m_shadowMapHeight = 0; r.SetShadowMapSize(512, 256);
    if (std::strcmp(g_seq, "RI") || r.m_shadowMapWidth != 512 || r.m_shadowMapHeight != 256 || (g_is32 && (char*)&r.m_shadowMapHeight - (char*)&r != 0x33c)) { std::printf("00b4fc10_TEST FAIL\n"); return 1; }
    std::printf("00b4fc10_TEST PASS\n");
    return 0;
}