#include <cstdio>
static const void* g_baseSrc = 0;
class CDefinitionManager { public: int x; };
class CResource { public: virtual void v00() { } unsigned char _body[0x18]; CResource() { } CResource(const CResource& o) { g_baseSrc = &o; } };
class CDefClassBase : public CResource { public:
    CDefinitionManager* PDefManager; unsigned long GlobalIndex;
    unsigned char Setup : 1; unsigned char Template : 1; unsigned char DefaultValsApplied : 1;
    CDefClassBase() { }
    CDefClassBase(const CDefClassBase& o) : CResource(o), PDefManager(o.PDefManager), GlobalIndex(o.GlobalIndex), Setup(o.Setup), Template(o.Template), DefaultValsApplied(o.DefaultValsApplied) { }
};
int main()
{
    CDefinitionManager m; CDefClassBase a; a.PDefManager = &m; a.GlobalIndex = 42; a.Setup = 1; a.Template = 0; a.DefaultValsApplied = 1;
    CDefClassBase b(a);
    if (g_baseSrc != &a || b.PDefManager != &m || b.GlobalIndex != 42 || b.Setup != 1 || b.Template != 0 || b.DefaultValsApplied != 1
        || (char*)&b.PDefManager - (char*)&b != 0x1c || sizeof(CDefClassBase) != 0x28) { std::printf("0046644c_TEST FAIL\n"); return 1; }
    std::printf("0046644c_TEST PASS\n"); return 0;
}