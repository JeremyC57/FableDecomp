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
};
FABLE_STATIC_ASSERT(sizeof(CSpecialAbilitiesHealLifeDef)==0x60);
#else
#include "engine/_quarantine/CSpecialAbilitiesHealLifeDef.h"
#endif
#endif
