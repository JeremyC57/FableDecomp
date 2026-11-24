// NF49 (round 7): the def and manager classes come from the shared headers (were local CCharString / CDefPointeeBase / CRegionDef / CCameraManagerSetDef / CDefinitionManager models with
// same-TU stand-in GetDef bodies over made-up Lookup_CDefinitionManager_GetDef / _GetDefByName callees): GetDef 0x004AB990 is the
// member template ??$GetDef@VCCameraManagerSetDef@@@CDefinitionManager@@QBE_NABVCCharString@@PAV?$CDefPointer@$$CBVCCameraManagerSetDef@@@@@Z
// and GetDef 0x004AB920 its by-index sibling ??$GetDef@VCCameraManagerSetDef@@@CDefinitionManager@@QBE_NJPAV?$CDefPointer@$$CBVCCameraManagerSetDef@@@@@Z
// (FableWin.pdb), both bodies visible and kept out of line (FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_NAME_BODY / _GETDEF_GLOBAL_BODY);
// the region's def handle is CRegion::GetPRegionDef() (engine/CRegion.h; engine/CRegionDef.h CameraManagerSetIndex +0x48) and the camera
// manager's setter takes const CDefPointer<const CCameraManagerSetDef>& (Ego_r.pdb / FableWin.pdb, retail 0x0069AC90; its
// same-TU stand-in body is the row's own, kept).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_NAME_BODY
#include "fable_string.h"
#include "engine/CDefClassBase.h"
#include "engine/CDefPointer.h"
#include "engine/CDefinitionManager.h"
#include "engine/CRegionDef.h"
#define FABLE_RETAIL_CREGION
#define FABLE_RETAIL_CREGION_PREGIONDEF
#include "engine/CRegion.h"   // NF49: the region (was a local model) and its inline GetPRegionDef (FableWin.pdb), copied in place as retail
#include "engine/CCameraManagerSetDef.h"
// CGameCameraManager::SetUpMainCameraManagerSetDef() -- use the current region's camera manager set (its def's
// CameraManagerSetIndex), or the CAMERA_MANAGER_SET_DEFAULT set when the region names none
// Leaf: ?SetUpMainCameraManagerSetDef@CGameCameraManager@@AAEXXZ
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
    void SetCameraManagerSetDef(const CDefPointer<const CCameraManagerSetDef>& def);   // 0x0069AC90
    private:
    void SetUpMainCameraManagerSetDef();
    public:
};
void Store_CGameCameraManager_SetCameraManagerSetDef(CGameCameraManager* manager, const CCameraManagerSetDef* def);   // stand-in callee
#pragma auto_inline(off)
__declspec(noinline) void CGameCameraManager::SetCameraManagerSetDef(const CDefPointer<const CCameraManagerSetDef>& def)
{
    Store_CGameCameraManager_SetCameraManagerSetDef(this, def.Get());
}
#pragma auto_inline(on)

void CGameCameraManager::SetUpMainCameraManagerSetDef()
{
    CWorld* world = World;
    CDefPointer<const CRegionDef> regionDef(world->PeekWorldMap()->GetRegion(world->PeekWorldMap()->GetLoadedRegion()).GetPRegionDef());
    if (regionDef.Get()) {
        CDefPointer<const CCameraManagerSetDef> setDef;
        if (regionDef->CameraManagerSetIndex > 0) {
            World->DefinitionManager->GetDef(regionDef->CameraManagerSetIndex, &setDef);
        } else {
            CCharString name("CAMERA_MANAGER_SET_DEFAULT", -1);
            World->DefinitionManager->GetDef(name, &setDef);
        }
        SetCameraManagerSetDef(setDef);
    }
}