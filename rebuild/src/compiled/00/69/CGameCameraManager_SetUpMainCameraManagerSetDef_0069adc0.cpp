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

class CDefPointeeBase { public:
    virtual void v00();
    virtual void Destroy();                         // slot 1
    long m_refCount;                                // +0x04
    void AddRef() { ++m_refCount; }
    void Release() { if (--m_refCount == 0) Destroy(); }
};
class CRegionDef : public CDefPointeeBase { public:
    unsigned char _pad08[0x40];
    long CameraManagerSetIndex;                     // +0x48
};
class CCameraManagerSetDef : public CDefPointeeBase { public:};

template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) {}
    CDefPointer(T* object) : Object(object) { if (Object) Object->AddRef(); }
    ~CDefPointer()
    {
        if (Object)
            Object->Release();
    }
};

class CDefinitionManager { public:
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

template <class T> class CDefPointer;
class CRegion { public:
    unsigned char _pad00[0x24];
    CDefPointer<CRegionDef> m_pDef;                 // +0x24
};
class CWorldMap { public:
    long GetLoadedRegion() const;                  // 0x004FB150
public:
    const CRegion& GetRegion(long) const;   // 0x004FC180
};
class CWorld { public:
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
    CDefPointer<CRegionDef> regionDef(world->PeekWorldMap()->GetRegion(world->PeekWorldMap()->GetLoadedRegion()).m_pDef.Object);
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