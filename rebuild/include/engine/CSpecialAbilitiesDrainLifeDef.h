#ifndef FABLE_ENGINE_CSPECIALABILITIESDRAINLIFEDEF_H
#define FABLE_ENGINE_CSPECIALABILITIESDRAINLIFEDEF_H
#ifdef FABLE_RETAIL_CDRAINLIFEDEF_PASS22
#include "engine/CParentDefClassBase.h"
#include "stlport/fable_containers.h"
#include "fable_string.h"
class CSpecialAbilitiesDrainLifeDef:public CParentDefClassBase {
public:
 std::vector<float> AmountOfHealthSucked;
 std::vector<long> NumEffects;
 std::vector<float> SpellRange;
 std::vector<CCharString> ShotName;
 float SpellDelaySecs;
};
FABLE_STATIC_ASSERT(offsetof(CSpecialAbilitiesDrainLifeDef,SpellRange)==0x54);
FABLE_STATIC_ASSERT(sizeof(CSpecialAbilitiesDrainLifeDef)==0x70);
#endif
#endif
// Retail opt-in (Codex pass22): both original PDBs,all members,actual parent and four12-byte native vectors.
