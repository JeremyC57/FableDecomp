#include <cstdio>
static int g_point = 0, g_spot = 0;
struct CEngineInternalPrimitiveBase { unsigned char _pad[8]; long m_type; };
struct CEngineLightingManager { void AddPointLightPrimitive(CEngineInternalPrimitiveBase*) { ++g_point; } void AddSpotLightPrimitive(CEngineInternalPrimitiveBase*) { ++g_spot; }
    long RenderPrimitive(CEngineInternalPrimitiveBase* p, long, long) { if (p->m_type == 15) { AddPointLightPrimitive(p); return 2; } if (p->m_type == 16) { AddSpotLightPrimitive(p); return 2; } return 0; } };
int main() {
    CEngineLightingManager m; CEngineInternalPrimitiveBase p; p.m_type = 15;
    if (m.RenderPrimitive(&p, 0, 0) != 2 || g_point != 1 || g_spot != 0) { std::printf("00b481e0_TEST FAIL\n"); return 1; }
    p.m_type = 16; if (m.RenderPrimitive(&p, 0, 0) != 2 || g_point != 1 || g_spot != 1) { std::printf("00b481e0_TEST FAIL\n"); return 1; }
    p.m_type = 3; if (m.RenderPrimitive(&p, 0, 0) != 0 || g_point != 1 || g_spot != 1) { std::printf("00b481e0_TEST FAIL\n"); return 1; }
    std::printf("00b481e0_TEST PASS\n"); return 0;
}