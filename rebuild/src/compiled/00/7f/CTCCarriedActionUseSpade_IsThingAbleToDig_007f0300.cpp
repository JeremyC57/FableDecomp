// CTCCarriedActionUseSpade::IsThingAbleToDig(const CThingCreatureBase& creature) -- a creature can dig only on bare ground: not standing on a thing, and the strongest engine theme under it has a material def flagged IsDiggable
// Leaf: ?IsThingAbleToDig@CTCCarriedActionUseSpade@@SI_NABVCThingCreatureBase@@@Z
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Naming wave 18: the global stand-in g_pMainGameComponent (0x013B86A0) is the protected static CMainGameComponent::PMainGameComponent, read through the
//   inline static accessor Get() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) @ 0x007F0300, 199 bytes (+ int3 padding). Ego_r.pdb / FableWin.pdb
//   `?IsThingAbleToDig@CTCCarriedActionUseSpade@@SI_NABVCThingCreatureBase@@@Z` (public static `__fastcall`); callers
//   CTCCarriedActionUseSpade 0x007F03D0 and CContextSensitiveItemDef::IsValidOverride 0x00850A80.
// Callees: the creature's virtual GetThingStandingOn (slot 39, +0x9c: CThing returns 0, CThingGameObject its
//   PThingStandingOn), CThing::GetPos 0x004C73D0, CWorld::PeekWorldMap (slot 13, +0x34),
//   CWorldMap::GetStrongestEngineThemeAt(const C3DVector&, long*) 0x0063ADF0, CDefinitionManager::GetDef<CEngineThemeDef>
//   0x0063F3A0 and GetDef<CMaterialDef> 0x0066C340 (manifest names both "<CThingPhysicalSwitchDef>": byte twins), the def's
//   virtual slot 1 (destroy).
// Globals: CMainGameComponent::Get() 0x013B86A0 (PWorld +0x24); CWorld DefinitionManager +0x10 (Ego_r.pdb).
// Layout: retail CEngineThemeDef MaterialDef +0x8c and CMaterialDef IsDiggable +0x4d (both 4 below Ego_r.pdb, as the landed
//   CTCStealth::UpdateViewability 0x00747480 finds for the theme def); def objects count references at +4.
// Shape: the whole test inside `if (!creature.GetThingStandingOn())` with one `return false` last; the world is a named
//   local (esi) and the def manager comes through an inline `GetDefinitionManager()` getter (an inline CALL as the object
//   expression is evaluated before the arguments -- edi, saved with ebp -- where the raw member is loaded after them; the
//   second GetDef re-reads it); the two CDefPointer locals
//   share one frame slot (the first enregistered in edi after its GetDef), which needs same-TU `auto_inline(off)` GetDef
//   stand-ins so VC7.1 sees the out pointers do not escape (GOTCHAS I-3 / the landed 0x0083C1E0 idiom); the
//   second def's dereference lets VC drop both destructors' null tests on the success path.
// NF49 (round 7): the def classes and the manager come from the shared headers (were local CDefPointeeBase / CEngineThemeDef /
// CMaterialDef / CDefPointer / CDefinitionManager models plus same-TU stand-in GetDef bodies over made-up
// FindDef_CDefinitionManager_GetDef_0 / _1 callees): the lookups are the member template instances
// ??$GetDef@VCEngineThemeDef@@@CDefinitionManager@@QBE_NJPAV?$CDefPointer@$$CBVCEngineThemeDef@@@@@Z (0x0063F3A0) and
// ??$GetDef@VCMaterialDef@@@CDefinitionManager@@QBE_NJPAV?$CDefPointer@$$CBVCMaterialDef@@@@@Z (0x0066C340) (FableWin.pdb), with the
// header's same-TU body (FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY, the stand-ins' role); CMaterialDef is engine/CMaterialDef.h
// (IsDiggable +0x4d); CEngineThemeDef has no shared header (engine/_quarantine only), so it stays modelled on the shared CDefClassBase.
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD   // NF341 r2: GetDef<CMaterialDef> copy = retail 0x0066C340 (the re-reading SetDynamicCast, as landed 0x0066B370)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#include "fable_string.h"
#include "engine/CDefClassBase.h"
#include "engine/CDefPointer.h"
#include "engine/CDefinitionManager.h"
#include "engine/CMaterialDef.h"
#include "engine/C3DVector.h"

class CEngineThemeDef : public CDefClassBase { public:            // Ego_r.pdb CEngineThemeDef (no shared header); retail offsets
    unsigned char _pad_0x25[0x8c - 0x25];
    long          MaterialDef;                                  // +0x8c
};


// NF56 (round 7, queue q10434): CWorldMap comes from engine/CWorldMap.h (opt-in FABLE_RETAIL_CWORLDMAP_STRONGESTTHEME), so the call binds
//   ?GetStrongestEngineThemeAt@CWorldMap@@QBEJABVC3DVector@@AAJ@Z (Ego_r.pdb 0x006E84E0, strength by reference; the landed 0x0063DD70 /
//   0x0063EB20 bind it); was a local CWorldMap model taking long*. The other local models are pre-existing.
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CWORLDMAP_STRONGESTTHEME
#include "engine/CWorldMap.h"

class CWorld {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12();
    virtual const CWorldMap& PeekWorldMap() const;             // slot 13 (+0x34)
    void*               Component;                              // +0x04 (Ego_r +0x08 after a second base vptr)
    unsigned char       _pad08[0x10 - 0x08];
    CDefinitionManager* DefinitionManager;                      // +0x10
    CDefinitionManager& GetDefinitionManager() const { return *DefinitionManager; }
};

class CMainGameComponent {
protected:
    static CMainGameComponent* PMainGameComponent;      // 0x013B86A0 ?PMainGameComponent@CMainGameComponent@@1PAV1@A (protected static; ESTABLISHED_SPELLINGS)
public:
    static CMainGameComponent* __fastcall Get() { return PMainGameComponent; }   // Ego_r / FableWin ?Get@CMainGameComponent@@SIPAV1@XZ (inline)
private: public:
    unsigned char _pad00[0x24];
    CWorld*       PWorld;                                       // +0x24 scoped_ptr<CWorld>
};

class CThing {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38();
    virtual CThing* GetThingStandingOn() const;                 // slot 39 (+0x9c)
    const C3DVector& GetPos() const;                       // 0x004C73D0
};
class CThingCreatureBase : public CThing { };

class CTCCarriedActionUseSpade {
public:
    static bool __fastcall IsThingAbleToDig(const CThingCreatureBase& creature);
};

bool __fastcall CTCCarriedActionUseSpade::IsThingAbleToDig(const CThingCreatureBase& creature)
{
    if (!creature.GetThingStandingOn()) {
        CWorld* world = CMainGameComponent::Get()->PWorld;
        long weight;
        CDefPointer<const CEngineThemeDef> themeDef;
        if (world->GetDefinitionManager().GetDef(world->PeekWorldMap().GetStrongestEngineThemeAt(creature.GetPos(), weight), &themeDef)) {
            CDefPointer<const CMaterialDef> materialDef;
            world->GetDefinitionManager().GetDef(themeDef->MaterialDef, &materialDef);
            if (materialDef->IsDiggable)
                return true;
        }
    }
    return false;
}