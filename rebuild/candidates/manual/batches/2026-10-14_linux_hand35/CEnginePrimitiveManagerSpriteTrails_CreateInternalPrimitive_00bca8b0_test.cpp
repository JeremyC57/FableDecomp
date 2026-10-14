#include <cstdio>
static int g_baseType = -1; static unsigned long g_baseFlags = 0;
struct CEngineInternalPrimitiveBase { virtual void v00() { } unsigned char _pad04[0x44]; CEngineInternalPrimitiveBase(long type, unsigned long flags) { g_baseType = type; g_baseFlags = flags; } };
struct CEngineInternalPrimitiveSpriteTrail : CEngineInternalPrimitiveBase { unsigned char _pad48[0x20]; long m_w[4]; unsigned char _rest[0x3c]; CEngineInternalPrimitiveSpriteTrail() : CEngineInternalPrimitiveBase(0x14, 0x200000) { for (int i = 0; i < 4; ++i) m_w[i] = 0; } };
struct CEnginePrimitiveManagerSpriteTrails { CEngineInternalPrimitiveBase* CreateInternalPrimitive(long type) { if (type != 0x14) return 0; return new CEngineInternalPrimitiveSpriteTrail; } };

int main() {
    CEnginePrimitiveManagerSpriteTrails m; if (m.CreateInternalPrimitive(0x14 + 1) || g_baseType != -1) { std::printf("00bca8b0_TEST FAIL\n"); return 1; }
    CEngineInternalPrimitiveSpriteTrail* p = (CEngineInternalPrimitiveSpriteTrail*)m.CreateInternalPrimitive(0x14); if (!p || g_baseType != 0x14 || g_baseFlags != 0x200000 || p->m_w[0] || p->m_w[3]) { std::printf("00bca8b0_TEST FAIL\n"); return 1; }
    delete p;
    std::printf("00bca8b0_TEST PASS\n"); return 0;
}
