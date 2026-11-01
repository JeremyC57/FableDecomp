// Behaviour test: CEnginePrimitiveRenderer::AddPrimitiveManager (0x00B33AE0) -- push a copy of the counted manager pointer
// (the shared count goes up) and register the raw manager with the type info.
#include <cstdio>
class CCPPointerInfo { public: unsigned long RefCount; };
class CEnginePrimitiveManager { public: int id; };
struct CCountedPointer {
    CEnginePrimitiveManager* Data; CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) { }
    CCountedPointer(const CCountedPointer& o) : Data(o.Data), Info(o.Info) { if (Info) ++Info->RefCount; }
    CEnginePrimitiveManager* Get() const { return Data; }
};
static CEnginePrimitiveManager* g_registered[4]; static int g_nreg = 0;
class CEnginePrimitiveTypeInfo { public: void RegisterPrimitiveManager(CEnginePrimitiveManager* m) { g_registered[g_nreg++] = m; } };
static CEnginePrimitiveTypeInfo g_typeInfo;
static CEnginePrimitiveTypeInfo* g_pEnginePrimitiveTypeInfo = &g_typeInfo;
struct CArray {                                  // fixed-capacity stand-in for the STLport vector
    CCountedPointer items[4]; int count;
    CArray() : count(0) { }
    void push_back(const CCountedPointer& x) { items[count].Data = x.Data; items[count].Info = x.Info; if (x.Info) ++x.Info->RefCount; ++count; }
};
class CEnginePrimitiveRenderer { public:
    CArray PrimitiveManagers;
    void AddPrimitiveManager(const CCountedPointer& manager)
    {
        PrimitiveManagers.push_back(manager);
        g_pEnginePrimitiveTypeInfo->RegisterPrimitiveManager(manager.Get());
    }
};
int main()
{
    CEnginePrimitiveManager m1, m2; CCPPointerInfo i1 = { 1 };
    CCountedPointer p1; p1.Data = &m1; p1.Info = &i1;
    CCountedPointer p2; p2.Data = &m2; p2.Info = 0;
    CEnginePrimitiveRenderer r;
    r.AddPrimitiveManager(p1);
    r.AddPrimitiveManager(p2);
    if (r.PrimitiveManagers.count != 2 || r.PrimitiveManagers.items[0].Data != &m1 || i1.RefCount != 2 ||
        r.PrimitiveManagers.items[1].Info != 0 || g_nreg != 2 || g_registered[0] != &m1 || g_registered[1] != &m2)
    { std::printf("00b33ae0_TEST FAIL\n"); return 1; }
    std::printf("00b33ae0_TEST PASS\n");
    return 0;
}