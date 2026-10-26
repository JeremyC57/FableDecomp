#include <cstdio>
struct SRenderSettings_01436e60 { unsigned char _pad[0x334]; long m_clothMode; }; static SRenderSettings_01436e60 g_settings; static SRenderSettings_01436e60* g_pRenderSettings_01436e60 = &g_settings; static int g_common = 0;
struct SMeshBuffer { void* m_pA; void* m_pB; };
class CEnginePrimitiveManagerRepeatedStaticMeshes { public: unsigned char _pad[0x40]; SMeshBuffer m_normal[4]; SMeshBuffer m_shadow[4]; SMeshBuffer* m_pCurrent[4]; void FrameStartCommon() { ++g_common; }
    void FrameStart() { if (g_pRenderSettings_01436e60->m_clothMode == 2) { m_pCurrent[0] = &m_shadow[0]; m_pCurrent[1] = &m_shadow[1]; m_pCurrent[2] = &m_shadow[2]; m_pCurrent[3] = &m_shadow[3]; } else { m_pCurrent[0] = &m_normal[0]; m_pCurrent[1] = &m_normal[1]; m_pCurrent[2] = &m_normal[2]; m_pCurrent[3] = &m_normal[3]; } FrameStartCommon(); } };
int main() {
    CEnginePrimitiveManagerRepeatedStaticMeshes* m = (CEnginePrimitiveManagerRepeatedStaticMeshes*)new char[sizeof(CEnginePrimitiveManagerRepeatedStaticMeshes)];
    g_settings.m_clothMode = 2; m->FrameStart(); if (g_common != 1 || m->m_pCurrent[0] != &m->m_shadow[0] || m->m_pCurrent[3] != &m->m_shadow[3]) { std::printf("00b99b50_TEST FAIL\n"); return 1; }
    g_settings.m_clothMode = 0; m->FrameStart(); if (g_common != 2 || m->m_pCurrent[0] != &m->m_normal[0] || m->m_pCurrent[2] != &m->m_normal[2]) { std::printf("00b99b50_TEST FAIL\n"); return 1; }
    delete[] (char*)m;
    std::printf("00b99b50_TEST PASS\n"); return 0;
}