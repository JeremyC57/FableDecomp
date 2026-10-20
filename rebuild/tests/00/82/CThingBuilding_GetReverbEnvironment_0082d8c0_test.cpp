// Behaviour test for 0082d8c0: body copied from src/0082d8c0.cpp (CThingBuilding::GetReverbEnvironment), callees stubbed.
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
    ~CDefPointer()
    {
        if (Object)
            Object->Release();
    }
};
static CThingBuildingDef* g_def = 0; static long g_askedIndex = -1; static int g_getDef = 0;
struct CDefinitionManager {
    bool GetDef(long globalIndex, CDefPointer<CThingBuildingDef>* out);
};
bool CDefinitionManager::GetDef(long globalIndex, CDefPointer<CThingBuildingDef>* out)
{
    ++g_getDef; g_askedIndex = globalIndex;
    if (!g_def) return false;
    ++g_def->m_refCount; out->Object = g_def; return true;
}
static CDefinitionManager g_manager;
struct CThingBuilding {
    unsigned char _pad00[0x8c];
    unsigned short DefGlobalIndex;
    CDefinitionManager* GetDefinitionManager() { return &g_manager; }
    const CThingBuildingDef* GetBuildingDef()
    {
        CDefPointer<CThingBuildingDef> def;
        GetDefinitionManager()->GetDef(DefGlobalIndex, &def);
        return def.Object;
    }
    void GetReverbEnvironment(long* type, long* level);
};
void CThingBuilding::GetReverbEnvironment(long* type, long* level)
{
    const CThingBuildingDef* def = GetBuildingDef();
    *type = def->ReverbEnvironmentType;
    *level = def->ReverbEnvironmentLevel;
}
int main()
{
    static CThingBuildingDef def; def.m_refCount = 1; def.ReverbEnvironmentType = 3; def.ReverbEnvironmentLevel = 0x3F000000; g_def = &def;
    CThingBuilding b; memset(&b, 0, sizeof(b)); b.DefGlobalIndex = 321;
    long type = -1, level = -1;
    b.GetReverbEnvironment(&type, &level);
    CHECK(type == 3 && level == 0x3F000000); CHECK(g_getDef == 1 && g_askedIndex == 321); CHECK(def.m_refCount == 1 && g_destroyed == 0);
    def.ReverbEnvironmentType = 7; def.ReverbEnvironmentLevel = 9; def.m_refCount = 0;
    b.GetReverbEnvironment(&type, &level);
    CHECK(type == 7 && level == 9); CHECK(g_destroyed == 1);
    if (fails == 0) printf("0082d8c0_TEST PASS\n"); else printf("0082d8c0_TEST FAIL\n");
    return fails;
}