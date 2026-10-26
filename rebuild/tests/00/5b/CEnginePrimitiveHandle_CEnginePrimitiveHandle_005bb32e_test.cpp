#include <cstdio>
#include <new>
static int g_base = 0;
class CBaseClass { public: virtual void v00() { } CBaseClass() { ++g_base; } };
struct CIVCountedPointer { void* Object; CIVCountedPointer() : Object(0) { } };
class CEnginePrimitiveHandle : public CBaseClass { public: CIVCountedPointer Data; CEnginePrimitiveHandle(long) { } };
int main()
{
    unsigned char raw[sizeof(CEnginePrimitiveHandle)]; for (int k = 0; k < (int)sizeof(raw); ++k) raw[k] = 0xAA;
    CEnginePrimitiveHandle* h = new (raw) CEnginePrimitiveHandle(123);
    if (sizeof(CEnginePrimitiveHandle) != 8 || g_base != 1 || h->Data.Object != 0) { std::printf("005bb32e_TEST FAIL\n"); return 1; }
    std::printf("005bb32e_TEST PASS\n"); return 0;
}