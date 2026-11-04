#ifndef FABLE_ENGINE_CTCTARGETINGAI_H
#define FABLE_ENGINE_CTCTARGETINGAI_H
// CTCTargetingAI (new, chunk 765): RTTI .?AVCTCTargetingAI@@, single base CTCTargetingBase at +0 (ghidra_out/rtti_bases.tsv; engine/
// CTCTargetingBase.h, 56 bytes). Ego_r.pdb members (struct_layouts_egor.tsv, 84 bytes): PBestEnemyCreatureForAttack +0x38,
// PScriptTargetOverride +0x40 (CIntelligentPointer<CThingCreatureBase>), ThingsToNotAttack (list<CIntelligentPointer<CThing> >,
// PDB 12 bytes, retail STLport 4) +0x48: retail ClearAllThingsToNotAttack 0x007F6160 clears +0x48; sizeof 0x4C.
// Name set: ?ClearAllThingsToNotAttack@CTCTargetingAI@@QAEXXZ. No virtual overrides are declared.
#ifndef FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE
#endif
#include "engine/CTCTargetingBase.h"
#include "engine/CIntelligentPointer.h"
#include "stlport/fable_containers.h"
class CThing;
class CTCTargetingAI : public CTCTargetingBase {
public:
    void ClearAllThingsToNotAttack();                                    // 0x007F6160
protected:
    CIntelligentPointer<CThingCreatureBase> PBestEnemyCreatureForAttack; // +0x38
    CIntelligentPointer<CThingCreatureBase> PScriptTargetOverride;       // +0x40
    std::list<CIntelligentPointer<CThing> > ThingsToNotAttack;           // +0x48
};
FABLE_STATIC_ASSERT(sizeof(CTCTargetingAI) == 0x4c);
#endif
