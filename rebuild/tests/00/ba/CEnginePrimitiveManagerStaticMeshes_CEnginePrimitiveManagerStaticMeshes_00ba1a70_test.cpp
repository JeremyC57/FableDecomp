#include <cstdio>
#include <cstring>
static long g_baseType = -1; static const char* g_name = 0; static long g_nameLen = 0; static int g_set = 0, g_destroyed = 0, g_setBeforeCount = 0;
struct CCharString { void* m_pData; CCharString(const char* t, long l) : m_pData((void*)t) { g_name = t; g_nameLen = l; } ~CCharString() { ++g_destroyed; } };
struct CEnginePrimitiveManagerStaticMeshesBase { virtual void v00() { } unsigned char _pad[0x310]; CEnginePrimitiveManagerStaticMeshesBase(long t) { g_baseType = t; } };
struct CEnginePrimitiveManagerStaticMeshes : CEnginePrimitiveManagerStaticMeshesBase { long m_count; void SetName(const CCharString& n) { ++g_set; if (m_count != 1) g_setBeforeCount = 1; }
    CEnginePrimitiveManagerStaticMeshes(long type) : CEnginePrimitiveManagerStaticMeshesBase(type) { m_count = 0; { CCharString name("EnableStaticMeshes", -1); SetName(name); } m_count = 1; } };
int main() {
    CEnginePrimitiveManagerStaticMeshes m(9); if (g_baseType != 9 || m.m_count != 1 || std::strcmp(g_name, "EnableStaticMeshes") || g_nameLen != -1 || g_set != 1 || g_destroyed != 1 || g_setBeforeCount != 1) { std::printf("00ba1a70_TEST FAIL\n"); return 1; }
    std::printf("00ba1a70_TEST PASS\n"); return 0;
}