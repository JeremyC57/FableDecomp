#include <cstdio>
static int g_baseType = -1; static unsigned long g_baseFlags = 0;
struct CEngineInternalPrimitiveBase { virtual void v00() { } unsigned char _pad04[0x44]; CEngineInternalPrimitiveBase(long type, unsigned long flags) { g_baseType = type; g_baseFlags = flags; } };
struct CEngineInternalPrimitiveLine : CEngineInternalPrimitiveBase { unsigned char _rest[52]; CEngineInternalPrimitiveLine() : CEngineInternalPrimitiveBase(6, 0x2000000) {  } };
struct CEnginePrimitiveManagerLines { CEngineInternalPrimitiveBase* CreateInternalPrimitive(long type) { if (type != 6) return 0; return new CEngineInternalPrimitiveLine; } };

int main() {
    CEnginePrimitiveManagerLines m; if (m.CreateInternalPrimitive(6 + 1) || g_baseType != -1) { std::printf("00bc7e50_TEST FAIL\n"); return 1; }
    CEngineInternalPrimitiveLine* p = (CEngineInternalPrimitiveLine*)m.CreateInternalPrimitive(6); if (!p || g_baseType != 6 || g_baseFlags != 0x2000000) { std::printf("00bc7e50_TEST FAIL\n"); return 1; }
    delete p;
    std::printf("00bc7e50_TEST PASS\n"); return 0;
}