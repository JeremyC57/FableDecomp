#ifndef FABLE_ENGINE_CSPECIALABILITIESHEALLIFEDEF_ORIGINAL_H
#define FABLE_ENGINE_CSPECIALABILITIESHEALLIFEDEF_ORIGINAL_H
// Both complete original PDBs: CParentDefClassBase owner. Retail vector offsets3c/48/54, sizeof60.
#ifdef FABLE_RETAIL_CTCHEALLIFE_START_ORIGINAL
#include "engine/CParentDefClassBase.h"
#include "stlport/fable_containers.h"
class CSpecialAbilitiesHealLifeDef:public CParentDefClassBase {
public:
 std::vector<float> HealthIncreasePerFrame;
 std::vector<float> TotalHealthIncrease;
 std::vector<float> RadiusOfEffect;
#ifdef FABLE_RETAIL_CSPECIALABILITIESHEALLIFEDEF_COPY_DECLS
    // Opt-in (NF337, 2026-11-24): declarations only, no layout change, for CSpecialAbilitiesHealLifeDef::Copy 0x0046012C, which tail-calls the out-of-line
    // operator= (retail `jmp` 0x00460131). ?Copy@CSpecialAbilitiesHealLifeDef@@UAEXPBVCDefClassBase@@@Z is the member the retail RTTI vftable placement
    // (ghidra_out/labels_rtti_port.tsv) names at 0x0046012C (slot 19; it overrides CDefClassBase::Copy: clang override probe, made-up-name
    // control fails); ??4CSpecialAbilitiesHealLifeDef@@QAEAAV0@ABV0@@Z (Ego_r.pdb + FableWin.pdb) is the operator= at 0x00460131.
public:
    virtual void Copy(const CDefClassBase*);
    CSpecialAbilitiesHealLifeDef& operator=(const CSpecialAbilitiesHealLifeDef&);
#endif
};
FABLE_STATIC_ASSERT_AT(13, sizeof(CSpecialAbilitiesHealLifeDef)==0x60);
#else
#include "engine/_quarantine/CSpecialAbilitiesHealLifeDef.h"
#endif
#endif
