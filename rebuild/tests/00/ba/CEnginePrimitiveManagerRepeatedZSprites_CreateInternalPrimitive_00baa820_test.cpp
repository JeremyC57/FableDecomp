#include <cstdio>
static int g_baseType = -1; static unsigned long g_baseFlags = 0;
struct CEngineInternalPrimitiveBase { virtual void v00() { } unsigned char _pad04[0x44]; CEngineInternalPrimitiveBase(long type, unsigned long flags) { g_baseType = type; g_baseFlags = flags; } };
struct CEngineInternalPrimitiveRepeatedZSprite : CEngineInternalPrimitiveBase { long m_48; long m_4c; unsigned char _pad50[0x14]; long m_w[4]; unsigned char _rest[0x20]; CEngineInternalPrimitiveRepeatedZSprite() : CEngineInternalPrimitiveBase(0x2b, 0x10) { m_48 = 0; m_4c = 0; for (int i = 0; i < 4; ++i) m_w[i] = 0; } };
struct CEnginePrimitiveManagerRepeatedZSprites { CEngineInternalPrimitiveBase* CreateInternalPrimitive(long type) {  return new CEngineInternalPrimitiveRepeatedZSprite; } };

int main() {
    CEnginePrimitiveManagerRepeatedZSprites m;
    CEngineInternalPrimitiveRepeatedZSprite* p = (CEngineInternalPrimitiveRepeatedZSprite*)m.CreateInternalPrimitive(0x2b); if (!p || g_baseType != 0x2b || g_baseFlags != 0x10 || p->m_48 || p->m_4c || p->m_w[0] || p->m_w[3]) { std::printf("00baa820_TEST FAIL\n"); return 1; }
    delete p;
    std::printf("00baa820_TEST PASS\n"); return 0;
}