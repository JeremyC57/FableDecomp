#include <cstdio>
static int g_baseType = -1; static unsigned long g_baseFlags = 0;
struct CEngineInternalPrimitiveBase { virtual void v00() { } unsigned char _pad04[0x44]; CEngineInternalPrimitiveBase(long type, unsigned long flags) { g_baseType = type; g_baseFlags = flags; } };
struct CEngineInternalPrimitiveWeatherMask : CEngineInternalPrimitiveBase { long m_w[4]; unsigned char _rest[0x50]; CEngineInternalPrimitiveWeatherMask() : CEngineInternalPrimitiveBase(0x1f, 0x200000) { for (int i = 0; i < 4; ++i) m_w[i] = 0; } };
struct CEnginePrimitiveManagerWeatherMask { CEngineInternalPrimitiveBase* CreateInternalPrimitive(long type) { if (type != 0x1f) return 0; return new CEngineInternalPrimitiveWeatherMask; } };

int main() {
    CEnginePrimitiveManagerWeatherMask m; if (m.CreateInternalPrimitive(0x1f + 1) || g_baseType != -1) { std::printf("00bb6a50_TEST FAIL\n"); return 1; }
    CEngineInternalPrimitiveWeatherMask* p = (CEngineInternalPrimitiveWeatherMask*)m.CreateInternalPrimitive(0x1f); if (!p || g_baseType != 0x1f || g_baseFlags != 0x200000 || p->m_w[0] || p->m_w[3]) { std::printf("00bb6a50_TEST FAIL\n"); return 1; }
    delete p;
    std::printf("00bb6a50_TEST PASS\n"); return 0;
}