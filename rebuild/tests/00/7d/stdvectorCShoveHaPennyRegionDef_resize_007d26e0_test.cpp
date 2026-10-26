// Behaviour test for vector<CShoveHaPennyRegionDef>::resize(n) (0x007D26E0): the default def has no manager and cleared status bits, and is destroyed after the call.
#include <cstdio>
static unsigned int g_n = 0; static int g_valueOk = 0; static int g_resDtors = 0;
class CResource { public: unsigned char _resource[0x18]; CResource() {} virtual ~CResource() { ++g_resDtors; } };
class CDefPointeeBase : public CResource { public: CDefPointeeBase() { } virtual ~CDefPointeeBase() { } };
class CDefinitionManager;
class CDefClassBase : public CDefPointeeBase { public:
    CDefinitionManager* PDefManager; unsigned long GlobalIndex;
    unsigned char Setup : 1; unsigned char Template : 1; unsigned char DefaultValsApplied : 1;
    CDefClassBase() : PDefManager(0) { Setup = 0; Template = 0; DefaultValsApplied = 0; }
};
class CShoveHaPennyRegionDef : public CDefClassBase { public: float Start; float Score; };
template<class T> struct TVector {
    void resize(unsigned int newSize, const T& value) { g_n = newSize; g_valueOk = value.PDefManager == 0 && !value.Setup && !value.Template && !value.DefaultValsApplied; }
    void resize(unsigned int newSize) { resize(newSize, T()); }
};
int main()
{
    TVector<CShoveHaPennyRegionDef> v;
    v.resize(4);
    if (g_n != 4 || !g_valueOk || g_resDtors != 1) { std::printf("007d26e0_TEST FAIL\n"); return 1; }
    std::printf("007d26e0_TEST PASS\n");
    return 0;
}