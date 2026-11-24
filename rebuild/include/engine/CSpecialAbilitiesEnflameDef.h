#ifndef FABLE_ENGINE_CSPECIALABILITIESENFLAMEDEF_H
#define FABLE_ENGINE_CSPECIALABILITIESENFLAMEDEF_H
// Retail class (Codex pass 12 row 0x008EDFF0, header wave 2026-11-03): RTTI base CParentDefClassBase (packed, 0x39), members in
// Ego_r.pdb order and spelling (ghidra_out/struct_layouts_egor.tsv, all 8 checked) with 12-byte STLport vectors; the
// CWillButtonHelperTimer ctor 0x008EDFF0 reads LevelUpTimings through CDefinitionManager::GetDef (graded RELOCATION_MATCH).
#ifndef FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#endif
#include "engine/CParentDefClassBase.h"
#include "stlport/fable_containers.h"
class CSpecialAbilitiesEnflameDef : public CParentDefClassBase {
public:
#ifdef FABLE_RETAIL_CSPECIALABILITIESENFLAMEDEF_TRANSFER
// Opt-in (NF40, 2026-11-16, row 0x00465258 CSpecialAbilitiesEnflameDef::Transfer): Ego_r.pdb / FableWin.pdb
// ?Transfer@CSpecialAbilitiesEnflameDef@@UAEXAAVCPersistContext@@@Z, the def's override of CDefClassBase::Transfer (virtual slot 18).
// Declaration only (the elaborated class needs no include); no layout change.
public: virtual void Transfer(class CPersistContext&);
public:
#endif
    float CamDist[4];
    float CamHeight[4];
    float CamFOV[4];
    float CamSwitchTime[4];
    float LevelUpTimings[3];
    float Damage[4];
    std::vector<float,std::allocator<float> > RadiusOfEffect;
    float EnflameCombatMultiplierFactor;
#ifdef FABLE_RETAIL_CSPECIALABILITIESENFLAMEDEF_ALLOC
// Opt-in (wfE11, 2026-11-15, row 0x0044C72B CGameDefinitionManager::InitAndCompile, which registers its address): the static allocator
// ?Alloc@CSpecialAbilitiesEnflameDef@@SIPAVCDefClassBase@@XZ (Ego_r.pdb + FableWin.pdb). Declaration only (elaborated `class`: needs no include); no layout change.
public: static class CDefClassBase* FABLE_FASTCALL Alloc();
#endif
#ifdef FABLE_RETAIL_CSPECIALABILITIESENFLAMEDEF_COPY_DECLS
    // Opt-in (NF386, 2026-11-24; NF337 form): declarations only, no layout change, for CSpecialAbilitiesEnflameDef::Copy 0x0045FE26, which tail-calls the
    // out-of-line operator= (retail `jmp` 0x0045FE2B). ?Copy@CSpecialAbilitiesEnflameDef@@UAEXPBVCDefClassBase@@@Z is the member the retail RTTI vftable
    // placement (ghidra_out/labels_rtti_port.tsv) names at 0x0045FE26 (it overrides CDefClassBase::Copy: clang override probe,
    // made-up-name control fails); ??4CSpecialAbilitiesEnflameDef@@QAEAAV0@ABV0@@Z (PDB name sets) is the landed operator= row at 0x0045FE2B.
public:
    virtual void Copy(const CDefClassBase*);
    CSpecialAbilitiesEnflameDef& operator=(const CSpecialAbilitiesEnflameDef&);
#endif
};
#endif
