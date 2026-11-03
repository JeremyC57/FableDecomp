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
    float CamDist[4];
    float CamHeight[4];
    float CamFOV[4];
    float CamSwitchTime[4];
    float LevelUpTimings[3];
    float Damage[4];
    std::vector<float,std::allocator<float> > RadiusOfEffect;
    float EnflameCombatMultiplierFactor;
};
#endif
