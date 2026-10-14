#include <cstdio>
static int g_baseType = -1; static unsigned long g_baseFlags = 0;
struct CEngineInternalPrimitiveBase { virtual void v00() { } unsigned char _pad04[0x30]; unsigned char PostRenderEnable; unsigned char _pad35[0x13];
    CEngineInternalPrimitiveBase(long type, unsigned long flags) { g_baseType = type; g_baseFlags = flags; PostRenderEnable = 0; } };
struct CEnginePrimitiveHandle { long m_id; CEngineInternalPrimitiveBase* m_pPrimitive; };
struct CEngineInternalPrimitiveDecal : CEngineInternalPrimitiveBase { unsigned char _body[0x4c]; CEngineInternalPrimitiveDecal() : CEngineInternalPrimitiveBase(0x28, 0x4000) { } };
struct CEnginePrimitiveManagerDecal { CEngineInternalPrimitiveBase* CreateInternalPrimitive(long type) { if (type != 0x28) return 0; return new CEngineInternalPrimitiveDecal; } };
int main() {
    CEnginePrimitiveManagerDecal m; if (m.CreateInternalPrimitive(0x27) || g_baseType != -1) { std::printf("00bba440_TEST FAIL\n"); return 1; }
    CEngineInternalPrimitiveBase* d = m.CreateInternalPrimitive(0x28); if (!d || g_baseType != 0x28 || g_baseFlags != 0x4000) { std::printf("00bba440_TEST FAIL\n"); return 1; }
    delete d;
    std::printf("00bba440_TEST PASS\n"); return 0;
}