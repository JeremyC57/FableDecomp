#include <cstdio>
static int g_baseType = -1; static unsigned long g_baseFlags = 0;
struct CEngineInternalPrimitiveBase { virtual void v00() { } unsigned char _pad04[0x44]; CEngineInternalPrimitiveBase(long type, unsigned long flags) { g_baseType = type; g_baseFlags = flags; } };
struct CEngineInternalPrimitiveFlareSprite : CEngineInternalPrimitiveBase { unsigned char _pad48[0x48]; long m_90; long m_94; CEngineInternalPrimitiveFlareSprite() : CEngineInternalPrimitiveBase(0x11, 0xc00000) { m_94 = -1; m_90 = 0; } };
struct CEnginePrimitiveManagerFlareSprites { CEngineInternalPrimitiveBase* CreateInternalPrimitive(long type) { if (type != 0x11) return 0; return new CEngineInternalPrimitiveFlareSprite; } };

int main() {
    CEnginePrimitiveManagerFlareSprites m; if (m.CreateInternalPrimitive(0x11 + 1) || g_baseType != -1) { std::printf("00bcdf50_TEST FAIL\n"); return 1; }
    CEngineInternalPrimitiveFlareSprite* p = (CEngineInternalPrimitiveFlareSprite*)m.CreateInternalPrimitive(0x11); if (!p || g_baseType != 0x11 || g_baseFlags != 0xc00000 || p->m_94 != -1 || p->m_90 != 0) { std::printf("00bcdf50_TEST FAIL\n"); return 1; }
    delete p;
    std::printf("00bcdf50_TEST PASS\n"); return 0;
}
