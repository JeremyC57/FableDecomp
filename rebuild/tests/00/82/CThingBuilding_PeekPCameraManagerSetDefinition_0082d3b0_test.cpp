// 0082d3b0 behaviour test: CThingBuilding::PeekPCameraManagerSetDefinition
#ifndef _MSC_VER
#define __forceinline inline
#define __declspec(x)
#include <stdint.h>
#endif
#include <stdio.h>
#include <string.h>
struct CDefClassBase {
    virtual void v00();
    virtual void Delete();                           // slot 1 (+0x04)
    long RefCount;                                   // +0x04
};
#pragma pack(push, 1)
struct CThingBuildingDef : CDefClassBase {
    unsigned char _pad08[0x88 - 0x08];
    unsigned long WallMaterial;                    // +0x88
    unsigned char _pad8c[0x9c - 0x8c];
    long CameraManagerSet;                         // +0x9c
};
#pragma pack(pop)
struct CCameraManagerSetDef : CDefClassBase { };
struct CRegionDef : CDefClassBase {
    unsigned char _pad08[0x4c - 0x08];
    long CameraManagerSet;                         // +0x4c
};
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                // 0x0099EAE0
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    CDefPointer(T* object) : Object(object)
    {
        if (object)
            ++static_cast<CDefClassBase*>(object)->RefCount;
    }
    CDefPointer(const CDefPointer& other) : Object(other.Object)
    {
        if (Object)
            ++Object->RefCount;
    }
    ~CDefPointer()
    {
        CDefClassBase* old = Object;                  // released through the unpacked base (the def classes pack to 1)
        if (old) {
            if (--old->RefCount == 0)
                old->Delete();
        }
    }
};
struct CDefinitionManager {
    bool GetDef(long index, CDefPointer<CThingBuildingDef>* out);   // 0x004AB840
    bool GetDef(long index, CDefPointer<CCameraManagerSetDef>* out);   // 0x004AB920
    bool GetDef(const CCharString& name, CDefPointer<CCameraManagerSetDef>* out);   // 0x004AB990
};
void* FindDef_CDefinitionManager_GetDef_1();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefinitionManager::GetDef(long index, CDefPointer<CThingBuildingDef>* out)
{
    void* def = FindDef_CDefinitionManager_GetDef_1();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)
void* FindDef_CDefinitionManager_GetDef_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefinitionManager::GetDef(long index, CDefPointer<CCameraManagerSetDef>* out)
{
    void* def = FindDef_CDefinitionManager_GetDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)
struct CRegion {
    unsigned char _pad00[0x24];
    CRegionDef* PDef;                              // +0x24 CDefPointer<const CRegionDef>
};
struct CWorldMap {
    long GetCurrentRegionIndex() const;            // 0x004FB150
    CRegion& GetRegion(long index);                // 0x004FC180
};
struct CWorld {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12();
    virtual CWorldMap* GetWorldMap();              // slot 13 (+0x34)
};
void* FindDef_CDefinitionManager_GetDef_2();   // stand-in callee of the stand-in body below
#pragma auto_inline(off)
__declspec(noinline) bool CDefinitionManager::GetDef(const CCharString& name, CDefPointer<CCameraManagerSetDef>* out)
{
    void* def = FindDef_CDefinitionManager_GetDef_2();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)
struct CThingBuilding {
    unsigned char  _pad00[0x8c];
    unsigned short DefGlobalIndex;                   // +0x8c
    CDefinitionManager* GetDefinitionManager() const;   // 0x004C7990
    CWorld* GetWorld() const;                       // 0x004C7AB0
    __forceinline const CThingBuildingDef* PeekPDef() const
    {
        CDefPointer<CThingBuildingDef> def;
        GetDefinitionManager()->GetDef(DefGlobalIndex, &def);
        return def.Object;
    }
    CDefPointer<CCameraManagerSetDef> PeekPCameraManagerSetDefinition() const;
};
CDefPointer<CCameraManagerSetDef> CThingBuilding::PeekPCameraManagerSetDefinition() const
{
    CDefPointer<CCameraManagerSetDef> def;
    if (PeekPDef()->CameraManagerSet > 0) {
        GetDefinitionManager()->GetDef(PeekPDef()->CameraManagerSet, &def);
        return def;
    }
    CDefPointer<CRegionDef> regionDef(GetWorld()->GetWorldMap()->GetRegion(GetWorld()->GetWorldMap()->GetCurrentRegionIndex()).PDef);
    const CRegionDef* rdef = regionDef.Object;
    if (rdef && rdef->CameraManagerSet > 0) {
        GetDefinitionManager()->GetDef(rdef->CameraManagerSet, &def);
        return def;
    }
    GetDefinitionManager()->GetDef("CAMERA_MANAGER_SET_BUILDING", &def);
    return def;
}

// ---- mocks ----
static int g_deleted = 0;
void CDefClassBase::v00() { }
void CDefClassBase::Delete() { ++g_deleted; }
static char g_name[64];
static int g_nameLive = 0;
CCharString::CCharString(const char* text, long) { PStringData = 0; strcpy(g_name, text); ++g_nameLive; }
CCharString::~CCharString() { --g_nameLive; }

static CThingBuildingDef g_building;
static CCameraManagerSetDef g_camIndex, g_camName;
static int g_calls[3];
void* FindDef_CDefinitionManager_GetDef_1() { ++g_calls[1]; return &g_building; }
void* FindDef_CDefinitionManager_GetDef_0() { ++g_calls[0]; return &g_camIndex; }
void* FindDef_CDefinitionManager_GetDef_2() { ++g_calls[2]; return &g_camName; }

static CDefinitionManager g_dm;
static CRegion g_region;
static long g_currentRegion = 0, g_requestedRegion = -1;
static CWorldMap g_worldMap;
long CWorldMap::GetCurrentRegionIndex() const { return g_currentRegion; }
CRegion& CWorldMap::GetRegion(long index) { g_requestedRegion = index; return g_region; }
void CWorld::v00() { } void CWorld::v01() { } void CWorld::v02() { } void CWorld::v03() { } void CWorld::v04() { }
void CWorld::v05() { } void CWorld::v06() { } void CWorld::v07() { } void CWorld::v08() { } void CWorld::v09() { }
void CWorld::v10() { } void CWorld::v11() { } void CWorld::v12() { }
CWorldMap* CWorld::GetWorldMap() { return &g_worldMap; }
static CWorld g_world;
CDefinitionManager* CThingBuilding::GetDefinitionManager() const { return &g_dm; }
CWorld* CThingBuilding::GetWorld() const { return &g_world; }

static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); ++g_fail; } } while (0)

static void* Run(long buildingSet, CRegionDef* region, long regionSet, long* regionRefAfter)
{
    memset(g_calls, 0, sizeof g_calls);
    g_name[0] = 0;
    g_building.RefCount = 100;
    g_building.CameraManagerSet = buildingSet;
    g_camIndex.RefCount = 100;
    g_camName.RefCount = 100;
    g_region.PDef = region;
    if (region) { region->RefCount = 10; region->CameraManagerSet = regionSet; }
    g_currentRegion = 42; g_requestedRegion = -1;
    CThingBuilding thing;
    thing.DefGlobalIndex = 7;
    void* result;
    {
        CDefPointer<CCameraManagerSetDef> p = thing.PeekPCameraManagerSetDefinition();
        result = p.Object;
    }
    if (region && regionRefAfter) *regionRefAfter = region->RefCount;
    return result;
}

int main()
{
    CRegionDef regionDef;
    long rc = -1;
    // 1. building def names a positive set: index lookup, region never consulted
    void* r = Run(3, &regionDef, 9, &rc);
    CHECK(r == &g_camIndex); CHECK(g_calls[0] == 1 && g_calls[2] == 0); CHECK(g_requestedRegion == -1); CHECK(rc == 10);
    // 2. building set 0, no region def: name fallback
    r = Run(0, 0, 0, 0);
    CHECK(r == &g_camName); CHECK(g_calls[2] == 1 && g_calls[0] == 0); CHECK(strcmp(g_name, "CAMERA_MANAGER_SET_BUILDING") == 0);
    CHECK(g_requestedRegion == 42); CHECK(g_nameLive == 0);
    // 3. building set 0, region def with positive set: index lookup, region ref balanced
    r = Run(0, &regionDef, 5, &rc);
    CHECK(r == &g_camIndex); CHECK(g_calls[0] == 1 && g_calls[2] == 0); CHECK(rc == 10); CHECK(g_requestedRegion == 42);
    // 4. building set 0, region set 0: name fallback, region ref balanced
    r = Run(0, &regionDef, 0, &rc);
    CHECK(r == &g_camName); CHECK(g_calls[2] == 1 && g_calls[0] == 0); CHECK(rc == 10);
    // 5. negative sets both: name fallback
    r = Run(-4, &regionDef, -1, &rc);
    CHECK(r == &g_camName); CHECK(rc == 10); CHECK(g_nameLive == 0);
    // 6. region def whose only reference is ours-to-release: dropping it deletes
    g_deleted = 0;
    memset(g_calls, 0, sizeof g_calls);
    g_building.RefCount = 100; g_building.CameraManagerSet = 0;
    regionDef.RefCount = 0; regionDef.CameraManagerSet = 0; g_region.PDef = &regionDef;
    { CThingBuilding t; t.DefGlobalIndex = 1; CDefPointer<CCameraManagerSetDef> p = t.PeekPCameraManagerSetDefinition(); }
    CHECK(g_deleted == 1);
    if (g_fail) { printf("0082d3b0_TEST FAIL %d\n", g_fail); return 1; }
    printf("0082d3b0_TEST PASS\n");
    return 0;
}