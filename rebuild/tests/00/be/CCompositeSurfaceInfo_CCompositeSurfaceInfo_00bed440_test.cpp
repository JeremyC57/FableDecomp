#include <cstdio>
static int g_baseCtors = 0;
class CBaseClass {
public:  virtual void v00() {} CBaseClass() { ++g_baseCtors; } };
struct CCountedPointer { void* m_pObject; void* m_pCount; CCountedPointer() : m_pObject(0), m_pCount(0) {} };
class CCompositeSurfaceInfo : public CBaseClass { public: virtual void v00() {} CCountedPointer Cache; CCompositeSurfaceInfo() : CBaseClass(), Cache() {} };

int main()
{
    CCompositeSurfaceInfo info;
    if (g_baseCtors != 1 || info.Cache.m_pObject != 0 || info.Cache.m_pCount != 0 || sizeof(info) != 12) { std::printf("00bed440_TEST FAIL\n"); return 1; }
    std::printf("00bed440_TEST PASS\n");
    return 0;
}