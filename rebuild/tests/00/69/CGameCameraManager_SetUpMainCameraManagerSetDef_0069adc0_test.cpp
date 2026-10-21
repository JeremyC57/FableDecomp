// Behaviour test for 0069adc0: src/0069adc0.cpp copied verbatim; SetUpMainCameraManagerSetDef takes the current
// region's def and, when there is one, looks up its CameraManagerSetIndex set def (or CAMERA_MANAGER_SET_DEFAULT when the
// index is not positive), hands it to SetCameraManagerSetDef and releases both defs.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// CGameCameraManager::SetUpMainCameraManagerSetDef() -- use the current region's camera manager set (its def's
// CameraManagerSetIndex), or the CAMERA_MANAGER_SET_DEFAULT set when the region names none
// Leaf: SetUpMainCameraManagerSetDef
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0069ADC0, length 192 bytes, `ret`.
// Callees: CWorld virtual slot 13 PeekWorldMap (+0x34), CWorldMap::GetCurrentRegionNumber @ 0x004FB150,
// CWorldMap::GetRegion(long) @ 0x004FC180, CDefinitionManager::GetDef<CCameraManagerSetDef>(long, CDefPointer*)
// @ 0x004AB920 and GetDef<CCameraManagerSetDef>(const CCharString&, CDefPointer*) @ 0x004AB990 (template bodies
// shared by ICF, labelled for other def types), CCharString::CCharString(const char*, long) @ 0x0099EBF0,
// ~CCharString @ 0x0099EAE0, CGameCameraManager::SetCameraManagerSetDef @ 0x0069AC90, both defs' virtual slot 1.
// String 0x01238F9C "CAMERA_MANAGER_SET_DEFAULT".
// Retail layout: CGameCameraManager World +0x74 (header agrees); CWorld definition manager +0x10; region record def
// pointer +0x24; CRegionDef::CameraManagerSetIndex +0x48 (PDB +0x50).
// Shape: World read twice in one expression through a `world` local; the region def held by a CDefPointer built from
// the raw data word (inline AddRef gives `inc [reg+4]`; its release needs no null test inside the `if`); the set def is
// a CDefPointer out slot zeroed after the index read; the default-name string in its own block. GetDef (both overloads)
// and SetCameraManagerSetDef are same-TU noinline stand-in bodies (retail's LTCG saw the callees keep no reference to
// the slot), so the set-def release is `mov ecx,[slot] ; dec [ecx+4] ; jne` with no re-read of the slot.

class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length);     // 0x0099EBF0
    ~CCharString();                                 // 0x0099EAE0
};

struct CDefPointeeBase {
    virtual void v00();
    virtual void Destroy();                         // slot 1
    long m_refCount;                                // +0x04
    void AddRef() { ++m_refCount; }
    void Release() { if (--m_refCount == 0) Destroy(); }
};
struct CRegionDef : CDefPointeeBase {
    unsigned char _pad08[0x40];
    long CameraManagerSetIndex;                     // +0x48
};
struct CCameraManagerSetDef : CDefPointeeBase {};

template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) {}
    CDefPointer(T* object) : Object(object) { if (Object) Object->AddRef(); }
    ~CDefPointer()
    {
        if (Object)
            Object->Release();
    }
};

struct CDefinitionManager {
    bool GetDef(long index, CDefPointer<CCameraManagerSetDef>* def);                 // 0x004AB920
    bool GetDef(const CCharString& name, CDefPointer<CCameraManagerSetDef>* def);    // 0x004AB990
};
void* Lookup_CDefinitionManager_GetDef(const void* manager, long index);   // stand-in callees
void* Lookup_CDefinitionManager_GetDefByName(const void* manager, const CCharString& name);
#pragma auto_inline(off)
__declspec(noinline) bool CDefinitionManager::GetDef(long index, CDefPointer<CCameraManagerSetDef>* def)
{
    void* found = Lookup_CDefinitionManager_GetDef(this, index);
    if (!found)
        return false;
    def->Object = (CCameraManagerSetDef*)found;
    return true;
}
#pragma auto_inline(off)
__declspec(noinline) bool CDefinitionManager::GetDef(const CCharString& name, CDefPointer<CCameraManagerSetDef>* def)
{
    void* found = Lookup_CDefinitionManager_GetDefByName(this, name);
    if (!found)
        return false;
    def->Object = (CCameraManagerSetDef*)found;
    return true;
}
#pragma auto_inline(on)

template <class T> struct CDefPointer;
struct CRegion {
    unsigned char _pad00[0x24];
    CDefPointer<CRegionDef> m_pDef;                 // +0x24
};
struct CWorldMap {
    long GetCurrentRegionNumber();                  // 0x004FB150
    CRegion& GetRegion(long region);                // 0x004FC180
};
struct CWorld {
    virtual void w000(); virtual void w001(); virtual void w002(); virtual void w003(); virtual void w004(); virtual void w005();
    virtual void w006(); virtual void w007(); virtual void w008(); virtual void w009(); virtual void w010(); virtual void w011();
    virtual CWorldMap* GetWorldMap();               // slot 12 (+0x30)
    virtual CWorldMap* PeekWorldMap();              // slot 13 (+0x34)
    unsigned char _pad04[0xc];
    CDefinitionManager* DefinitionManager;          // +0x10
};

class CGameCameraManager {
public:
    virtual void v00();
    unsigned char _pad004[0x70];
    CWorld* World;                                  // +0x74
    void SetCameraManagerSetDef(const CDefPointer<CCameraManagerSetDef>& def);   // 0x0069AC90
    void SetUpMainCameraManagerSetDef();
};
void Store_CGameCameraManager_SetCameraManagerSetDef(CGameCameraManager* manager, CCameraManagerSetDef* def);   // stand-in callee
#pragma auto_inline(off)
__declspec(noinline) void CGameCameraManager::SetCameraManagerSetDef(const CDefPointer<CCameraManagerSetDef>& def)
{
    Store_CGameCameraManager_SetCameraManagerSetDef(this, def.Object);
}
#pragma auto_inline(on)

void CGameCameraManager::SetUpMainCameraManagerSetDef()
{
    CWorld* world = World;
    CDefPointer<CRegionDef> regionDef(world->PeekWorldMap()->GetRegion(world->PeekWorldMap()->GetCurrentRegionNumber()).m_pDef.Object);
    if (regionDef.Object) {
        CDefPointer<CCameraManagerSetDef> setDef;
        if (regionDef.Object->CameraManagerSetIndex > 0) {
            World->DefinitionManager->GetDef(regionDef.Object->CameraManagerSetIndex, &setDef);
        } else {
            CCharString name("CAMERA_MANAGER_SET_DEFAULT", -1);
            World->DefinitionManager->GetDef(name, &setDef);
        }
        SetCameraManagerSetDef(setDef);
    }
}
// ---- harness ----
static int g_destroyed = 0; static long g_index = -99; static char g_name[64]; static int g_set = 0;
static void* g_found = 0; static CCameraManagerSetDef* g_setDef = 0; static long g_setRefs = -1;
static long g_regionNumber = 7; static long g_askedRegion = -1; static int g_peeks = 0;
static CRegion g_region; static CWorldMap g_map; static CDefinitionManager g_manager;
CCharString::CCharString(const char* text, long length) { PStringData = (void*)text; (void)length; }
CCharString::~CCharString() { }
void CDefPointeeBase::v00() { }
void CDefPointeeBase::Destroy() { ++g_destroyed; }
long CWorldMap::GetCurrentRegionNumber() { return g_regionNumber; }
CRegion& CWorldMap::GetRegion(long region) { g_askedRegion = region; return g_region; }
void CWorld::w000() { } void CWorld::w001() { } void CWorld::w002() { } void CWorld::w003() { } void CWorld::w004() { }
void CWorld::w005() { } void CWorld::w006() { } void CWorld::w007() { } void CWorld::w008() { } void CWorld::w009() { }
void CWorld::w010() { } void CWorld::w011() { }
CWorldMap* CWorld::GetWorldMap() { return 0; }
CWorldMap* CWorld::PeekWorldMap() { ++g_peeks; return &g_map; }
void CGameCameraManager::v00() { }
void* Lookup_CDefinitionManager_GetDef(const void* manager, long index)
{ CHECK(manager == &g_manager); g_index = index; if (g_found) ((CDefPointeeBase*)g_found)->AddRef(); return g_found; }
void* Lookup_CDefinitionManager_GetDefByName(const void* manager, const CCharString& name)
{ CHECK(manager == &g_manager); strcpy(g_name, (const char*)name.PStringData); if (g_found) ((CDefPointeeBase*)g_found)->AddRef(); return g_found; }
void Store_CGameCameraManager_SetCameraManagerSetDef(CGameCameraManager* manager, CCameraManagerSetDef* def)
{ (void)manager; ++g_set; g_setDef = def; g_setRefs = def ? def->m_refCount : -1; }
int main()
{
    static CWorld world; world.DefinitionManager = &g_manager;
    static unsigned char camBuf[sizeof(CGameCameraManager)]; CGameCameraManager& cam = *(CGameCameraManager*)camBuf; cam.World = &world;
    static CRegionDef regionDef; regionDef.m_refCount = 1;
    static CCameraManagerSetDef setDef; setDef.m_refCount = 1;

    // no region def: nothing looked up
    g_region.m_pDef.Object = 0;
    cam.SetUpMainCameraManagerSetDef();
    CHECK(g_set == 0 && g_peeks == 2 && g_askedRegion == 7);

    // positive index: looked up by index, passed on with the extra reference, both released
    g_region.m_pDef.Object = &regionDef; regionDef.CameraManagerSetIndex = 0x31; g_found = &setDef;
    cam.SetUpMainCameraManagerSetDef();
    CHECK(g_set == 1 && g_index == 0x31 && g_setDef == &setDef && g_setRefs == 2 && g_name[0] == 0);
    CHECK(regionDef.m_refCount == 1 && setDef.m_refCount == 1 && g_destroyed == 0);

    // zero index: the default set by name
    regionDef.CameraManagerSetIndex = 0; g_index = -99;
    cam.SetUpMainCameraManagerSetDef();
    CHECK(g_set == 2 && g_index == -99 && strcmp(g_name, "CAMERA_MANAGER_SET_DEFAULT") == 0 && g_setDef == &setDef);
    CHECK(regionDef.m_refCount == 1 && setDef.m_refCount == 1 && g_destroyed == 0);

    // lookup fails: a null def is passed on
    g_found = 0; regionDef.CameraManagerSetIndex = 3;
    cam.SetUpMainCameraManagerSetDef();
    CHECK(g_set == 3 && g_setDef == 0 && g_index == 3 && regionDef.m_refCount == 1 && g_destroyed == 0);

    // last references: both destroyed
    g_found = &setDef; regionDef.m_refCount = 0; setDef.m_refCount = 0;
    cam.SetUpMainCameraManagerSetDef();
    CHECK(g_set == 4 && g_destroyed == 2);

    if (fails == 0) printf("0069adc0_TEST PASS\n");
    return fails;
}