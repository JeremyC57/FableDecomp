#include <cstdio>
static int g_baseType = -1; static unsigned long g_baseFlags = 0;
class CEngineInternalPrimitiveBase { public: virtual void v00() { } unsigned char _pad04[0x44]; CEngineInternalPrimitiveBase(long type, unsigned long flags) { g_baseType = type; g_baseFlags = flags; } };
class CEngineInternalPrimitiveSprite : public CEngineInternalPrimitiveBase { public: unsigned char _rest[64]; CEngineInternalPrimitiveSprite() : CEngineInternalPrimitiveBase(7, 0x200000) {  } };
class CEnginePrimitiveManagerSprites { public: CEngineInternalPrimitiveBase* CreateInternalPrimitive(long type) { if (type != 7) return 0; return new CEngineInternalPrimitiveSprite; } };

int main() {
    CEnginePrimitiveManagerSprites m; if (m.CreateInternalPrimitive(7 + 1) || g_baseType != -1) { std::printf("00bacf90_TEST FAIL\n"); return 1; }
    CEngineInternalPrimitiveSprite* p = (CEngineInternalPrimitiveSprite*)m.CreateInternalPrimitive(7); if (!p || g_baseType != 7 || g_baseFlags != 0x200000) { std::printf("00bacf90_TEST FAIL\n"); return 1; }
    delete p;
    std::printf("00bacf90_TEST PASS\n"); return 0;
}