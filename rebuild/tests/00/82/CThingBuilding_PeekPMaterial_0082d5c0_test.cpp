// Behaviour test for 0082d5c0: body copied from src/0082d5c0.cpp (CThingBuilding::PeekPMaterial), callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
static int g_deleted = 0;
struct CDefClassBase {
    virtual void v00() {}
    virtual void Delete() { ++g_deleted; }
    long RefCount;
};
#pragma pack(push, 1)
struct CThingBuildingDef : CDefClassBase {
    unsigned char _pad08[0x88 - 0x08];
    unsigned long WallMaterial;
};
#pragma pack(pop)
struct CWallMaterial : CDefClassBase { };
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    CDefPointer(const CDefPointer& other) : Object(other.Object)
    {
        if (Object)
            ++Object->RefCount;
    }
    ~CDefPointer()
    {
        CDefClassBase* old = Object;
        if (old) {
            if (--old->RefCount == 0)
                old->Delete();
        }
    }
};
static CThingBuildingDef* g_buildingDef = 0; static CWallMaterial* g_subDef = 0; static int g_getBuilding = 0, g_getSub = 0; static long g_buildingIndex = -1, g_subIndex = -1;
struct CDefinitionManager {
    bool GetDef(long index, CDefPointer<CThingBuildingDef>* out)
    {
        ++g_getBuilding; g_buildingIndex = index;
        if (!g_buildingDef) return false;
        ++g_buildingDef->RefCount; out->Object = g_buildingDef; return true;
    }
    bool GetDef(long index, CDefPointer<CWallMaterial>* out)
    {
        ++g_getSub; g_subIndex = index;
        if (!g_subDef) return false;
        ++g_subDef->RefCount; out->Object = g_subDef; return true;
    }
};
static CDefinitionManager g_manager;
struct CThingBuilding {
    unsigned char  _pad00[0x8c];
    unsigned short DefGlobalIndex;
    CDefinitionManager* GetDefinitionManager() const { return &g_manager; }
    const CThingBuildingDef* PeekPDef() const
    {
        CDefPointer<CThingBuildingDef> def;
        GetDefinitionManager()->GetDef(DefGlobalIndex, &def);
        return def.Object;
    }
    CDefPointer<CWallMaterial> PeekPMaterial() const;
};
CDefPointer<CWallMaterial> CThingBuilding::PeekPMaterial() const
{
    if (PeekPDef()->WallMaterial) {
        CDefPointer<CWallMaterial> sim;
        GetDefinitionManager()->GetDef(PeekPDef()->WallMaterial, &sim);
        return sim;
    }
    return CDefPointer<CWallMaterial>();
}
int main()
{
    static CThingBuildingDef bdef; static CWallMaterial sub; bdef.RefCount = 1; sub.RefCount = 1; g_buildingDef = &bdef; g_subDef = &sub;
    CThingBuilding b; memset(&b, 0, sizeof(b)); b.DefGlobalIndex = 0x77;
    // the def names a sub-def: resolved, the result holds one reference
    bdef.WallMaterial = 0x55;
    {
        CDefPointer<CWallMaterial> r = b.PeekPMaterial();
        CHECK(r.Object == &sub && sub.RefCount == 2); CHECK(g_getBuilding == 2 && g_buildingIndex == 0x77 && g_getSub == 1 && g_subIndex == 0x55); CHECK(bdef.RefCount == 1 && g_deleted == 0);
    }
    CHECK(sub.RefCount == 1);
    // the def names nothing: a null pointer, no second lookup
    bdef.WallMaterial = 0;
    { CDefPointer<CWallMaterial> r = b.PeekPMaterial(); CHECK(r.Object == 0 && g_getSub == 1 && g_getBuilding == 3); }
    // sub-def lookup fails: a null pointer
    bdef.WallMaterial = 0x55; g_subDef = 0;
    { CDefPointer<CWallMaterial> r = b.PeekPMaterial(); CHECK(r.Object == 0 && g_getSub == 2); }
    // a building def held only by the lookup is deleted after each peek
    bdef.RefCount = 0; bdef.WallMaterial = 0; { CDefPointer<CWallMaterial> r = b.PeekPMaterial(); } CHECK(g_deleted == 1);
    if (fails == 0) printf("0082d5c0_TEST PASS\n"); else printf("0082d5c0_TEST FAIL\n");
    return fails;
}