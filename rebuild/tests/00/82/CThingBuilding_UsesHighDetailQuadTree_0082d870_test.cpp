// Behaviour test for 0082d870: body copied from src/0082d870.cpp (CThingBuilding::UsesHighDetailQuadTree), callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
static int g_destroyed = 0;
struct CDefPointeeBase {
    virtual void v00() {}
    virtual void Destroy() { ++g_destroyed; }
    long m_refCount;
    void Release()
    {
        if (--m_refCount == 0)
            Destroy();
    }
};
struct CThingBuildingDef : CDefPointeeBase {
    unsigned char _pad08[0x80];
    long WallMaterial;
    long FloorMaterial;
    long ReverbEnvironmentType;
    long ReverbEnvironmentLevel;
    long OutsideOcclusionLevel;
    long CameraManagerSetIndex;
    unsigned char UseHighDetailQuadTree : 1;
    unsigned char IsSelfIlluminating : 1;
    unsigned char IsPartOfHeroGuild : 1;
    unsigned char HasWeatherMask : 1;
};
template<class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
};
static CThingBuildingDef* g_def = 0; static long g_askedIndex = -1; static int g_getDef = 0;
struct CDefinitionManager {
    bool GetDef(long globalIndex, CDefPointer<CThingBuildingDef>* out);
};
bool CDefinitionManager::GetDef(long globalIndex, CDefPointer<CThingBuildingDef>* out)
{
    ++g_getDef; g_askedIndex = globalIndex;
    if (!g_def)
        return false;
    ++g_def->m_refCount;
    out->Object = g_def;
    return true;
}
static CDefinitionManager g_manager;
struct CThingBuilding {
    unsigned char _pad00[0x8c];
    unsigned short DefGlobalIndex;
    CDefinitionManager* GetDefinitionManager() { return &g_manager; }
    __forceinline const CThingBuildingDef* GetBuildingDef()
    {
        CDefPointer<CThingBuildingDef> def;
        GetDefinitionManager()->GetDef(DefGlobalIndex, &def);
        CThingBuildingDef* object = def.Object;
        def.Object = 0;
        if (object)
            object->Release();
        return object;
    }
    unsigned char UsesHighDetailQuadTree();
};
unsigned char CThingBuilding::UsesHighDetailQuadTree()
{
    return GetBuildingDef()->UseHighDetailQuadTree;
}
int main()
{
    static CThingBuildingDef def; memset(&def._pad08, 0, sizeof(def._pad08)); def.m_refCount = 1; def.UseHighDetailQuadTree = 1; def.IsSelfIlluminating = 1; g_def = &def;
    CThingBuilding b; memset(&b, 0, sizeof(b)); b.DefGlobalIndex = 0x1234;
    CHECK(b.UsesHighDetailQuadTree() == 1); CHECK(g_getDef == 1 && g_askedIndex == 0x1234); CHECK(def.m_refCount == 1 && g_destroyed == 0);
    def.UseHighDetailQuadTree = 0; CHECK(b.UsesHighDetailQuadTree() == 0);
    // a def whose only reference was the lookup is destroyed by the release (the bit is still read from the object)
    def.m_refCount = 0; def.UseHighDetailQuadTree = 1; CHECK(b.UsesHighDetailQuadTree() == 1); CHECK(g_destroyed == 1 && def.m_refCount == 0);
    if (fails == 0) printf("0082d870_TEST PASS\n"); else printf("0082d870_TEST FAIL\n");
    return fails;
}