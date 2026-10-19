// CGameCameraManager::SetUpMainCameraManagerSetDef() -- use the current region's camera manager set (its def's
// Grade: DIFFER(ours v retail) -- 193v177: this/region-def registers swapped (this in esi, def in edi; retail this edi, World then def in esi) and the set-def dtor's inline Release() expands to the load/dec/store DecRef shape where retail has `mov ecx,[slot] ; dec [ecx+4] ; jne`
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
// Shape: World read twice in one expression; the region def held by a CDefPointer built from the raw data word (inline
// AddRef gives `inc [reg+4]`; its release needs no null test inside the `if`); the set def is a CDefPointer out slot
// zeroed after the index read; the default-name string in its own block.

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

void CGameCameraManager::SetUpMainCameraManagerSetDef()
{
    CDefPointer<CRegionDef> regionDef(World->PeekWorldMap()->GetRegion(World->PeekWorldMap()->GetCurrentRegionNumber()).m_pDef.Object);
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
