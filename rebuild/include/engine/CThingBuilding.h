#ifndef FABLE_ENGINE_CTHINGBUILDING_H
#define FABLE_ENGINE_CTHINGBUILDING_H
#include "engine/CThingGameObject.h"
// Ego_r TPI inheritance, members and access; native IsPointInside 0082d000.
#ifdef FABLE_RETAIL_CTHINGBUILDING_PEEKSIMDEF
#include "engine/CDefPointer.h"
class CSimBuildingDef;
#endif
#ifdef FABLE_RETAIL_CTHINGBUILDING_DEFINITION
// Retail opt-in (agentNF9): the definition accessors and the virtual overrides of rows 0x0082D3B0..0x0082D8C0. Needs
// FABLE_RETAIL_CTHINGBUILDINGDEF, FABLE_RETAIL_CTHING_MOREACCESS (CThing::PeekDefinitionManager) and the CGameDefinitionManager
// chain (FABLE_RETAIL_CDEFINITIONMANAGER, FABLE_RETAIL_CGAMEDEFINITIONMANAGER, FABLE_RETAIL_CRESOURCELIST, FABLE_RETAIL_CRESOURCE).
#include "engine/CDefPointer.h"
#include "engine/CThingBuildingDef.h"
#include "engine/CGameDefinitionManager.h"
class CCameraManagerSetDef; class CMaterialDef; class CEngineGraphic;
#endif
class CThingBuilding : public CThingGameObject {
protected:
    long ForceOpenCount;
#ifdef FABLE_RETAIL_CTHINGBUILDING
    // Retail opt-in (2026-11-03, chunk 720 row 0x0065B854 CTCVillage::AddThingToVillage): Ego_r.pdb types the member `bool:1`
    // (struct_layouts_egor.tsv CThingBuilding +0xEC bool:1), and retail sets it with `or byte ptr [esi+0D4h],1`
    // (0x0065BC42) through FableWin.pdb's inline ?SetOverrideAutomaticLocking@CThingBuilding@@QAEX_N@Z (0x0215ED7D, emitted
    // in the CTCVillage TU right after AddThingToVillage). Same offset and size as the default bool.
    bool OverrideAutomaticLocking : 1;
#else
    bool OverrideAutomaticLocking;
#endif
public:
    static long __fastcall StatGetType() { return 3; }
#ifdef FABLE_RETAIL_CTHINGBUILDING_TYPEFLAGS
    static unsigned long FABLE_FASTCALL StatGetTypeFlags(){return 0x47;}
#endif
#ifdef FABLE_RETAIL_CTHINGBUILDING
    // Header wave 2026-11-03 (row 0x00522A20): Ego_r.pdb ?Alloc@CThingBuilding@@SIPAVCClass@@XZ.
    static CClass* __fastcall Alloc();
#endif
#ifdef FABLE_RETAIL_CTHINGBUILDING_PEEKSIMDEF
    // Both PDBs and native 0082d2d0, caller 0090bc1c.
    CDefPointer<const CSimBuildingDef> PeekPSimDefinition() const;
#endif
    bool IsPointInside(const C3DVector&) const;
#ifdef FABLE_RETAIL_CTHINGBUILDING_DEFINITION
    // Retail opt-in (agentNF9). Spellings: Ego_r.pdb / FableWin.pdb name sets, all `@CThingBuilding@@`; Ego_r donor order
    // PeekDefinition 0x00558D00, PeekPSimDefinition, PeekPCameraManagerSetDefinition, GetCurrentGraphic, PeekPMaterial,
    // GetMaterialOnHitByMelee, OnCreate, GetFloorMaterial, UseHighDetailQuadTree, GetReverbSettings = retail 0x0082D2D0, 0x0082D3B0,
    // -, 0x0082D5C0, 0x0082D6B0, 0x0082D6F0, 0x0082D7D0, 0x0082D870, 0x0082D8C0; the virtuals are vtable 0x0124509C slots 8 (OnCreate),
    // 40 (GetFloorMaterial, CThing ?GetFloorMaterial@CThing@@UBEJXZ), 57 (UseHighDetailQuadTree, CThingPhysical's), 58 (PeekPMaterial,
    // CThingPhysical's) and 70 (GetMaterialOnHitByMelee, CThingGameObject's) (rebuild/manifest/rtti-vtables.tsv). Declarations only,
    // except PeekDefinition: ?PeekDefinition@CThingBuilding@@IBEABVCThingBuildingDef@@XZ has no out-of-line copy in Fable.exe (the
    // rows expand it: the counted handle is filled and released before the member read).
protected:
    const CThingBuildingDef& PeekDefinition() const
    {
        CDefPointer<const CThingBuildingDef> def;
        PeekDefinitionManager().GetDef(DefGlobalIndex, &def);
        return *def;
    }
    const CEngineGraphic* GetCurrentGraphic() const;                     // ?GetCurrentGraphic@CThingBuilding@@IBEPBVCEngineGraphic@@XZ
    virtual bool OnCreate();                                             // 0x0082D6F0 ?OnCreate@CThingBuilding@@MAE_NXZ
    virtual CDefPointer<const CMaterialDef> PeekPMaterial() const;       // 0x0082D5C0 ?PeekPMaterial@CThingBuilding@@MBE?AV?$CDefPointer@$$CBVCMaterialDef@@@@XZ
public:
    CDefPointer<const CCameraManagerSetDef> PeekPCameraManagerSetDefinition() const;   // 0x0082D3B0
    virtual long GetMaterialOnHitByMelee() const;                        // 0x0082D6B0 ?GetMaterialOnHitByMelee@CThingBuilding@@UBEJXZ
    virtual long GetFloorMaterial() const;                               // 0x0082D7D0 ?GetFloorMaterial@CThingBuilding@@UBEJXZ
    virtual bool UseHighDetailQuadTree() const;                          // 0x0082D870 ?UseHighDetailQuadTree@CThingBuilding@@UBE_NXZ
    void GetReverbSettings(EReverbEnvironmentType& type, float& level) const;   // 0x0082D8C0 ?GetReverbSettings@CThingBuilding@@QBEXAAW4EReverbEnvironmentType@@AAM@Z
#endif
#ifdef FABLE_RETAIL_CTHINGBUILDING_FORCEOPENCOUNT
    // Retail opt-in (chunk Q32, row 0x00958B20 CAIStateGroup_MinionReactToCrime::OnReplace): Ego_r.pdb / FableWin.pdb
    // ?DecrementForceOpenCount@CThingBuilding@@QAEXXZ, inline in retail (`dec dword ptr [eax+0D0h]` on ForceOpenCount, 0x00958B4A).
    void DecrementForceOpenCount() { --ForceOpenCount; }
#ifdef FABLE_RETAIL_CTHINGBUILDING_INCREMENTFORCEOPENCOUNT
    // Nested opt-in (wf17, row 0x00958840): FableWin.pdb ?IncrementForceOpenCount@CThingBuilding@@QAEXXZ, inline in retail (`inc [eax+0D0h]`).
    void IncrementForceOpenCount() { ++ForceOpenCount; }
#endif
#endif
#ifdef FABLE_RETAIL_CTHINGBUILDING
    void SetOverrideAutomaticLocking(bool value) { OverrideAutomaticLocking = value; }   // FableWin ?SetOverrideAutomaticLocking@CThingBuilding@@QAEX_N@Z (inline)
    void LockAllDoors(bool lock, bool immediately);                  // 0x0082DB00 ?LockAllDoors@CThingBuilding@@QAEX_N0@Z (Ego_r / FableWin)
#endif
#ifdef FABLE_RETAIL_CTHINGBUILDING_VSLOTS
    // agentFAM03 (2026-11-11): retail virtuals missing from this declaration, declaration only (no layout change).
    // Slots: the TLC vtable aligned with the Xbox 360 Test PDB order on TLC-named slots; spellings: Ego_r / FableWin name sets.
public:
    virtual bool DrawIsDisplayingInformation() const;  // slot 33, 0x0082D070
public:
    virtual bool IsAffectedByDamage() const;  // slot 47, 0x00529990
public:
    virtual unsigned long GetProperties() const;  // slot 48, 0x0082D0B0
public:
#endif
};
FABLE_STATIC_ASSERT(sizeof(CThingBuilding)==0xd8);
#endif
