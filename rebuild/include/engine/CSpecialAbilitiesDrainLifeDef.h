#ifndef FABLE_ENGINE_CSPECIALABILITIESDRAINLIFEDEF_H
#define FABLE_ENGINE_CSPECIALABILITIESDRAINLIFEDEF_H
#ifdef FABLE_RETAIL_CDRAINLIFEDEF_PASS22
#include "engine/CParentDefClassBase.h"
#include "stlport/fable_containers.h"
#include "fable_string.h"
class CSpecialAbilitiesDrainLifeDef:public CParentDefClassBase {
public:
#ifdef FABLE_RETAIL_CSPECIALABILITIESDRAINLIFEDEF_TRANSFER
// Opt-in (NF40, 2026-11-16, row 0x004724C2 CSpecialAbilitiesDrainLifeDef::Transfer): Ego_r.pdb / FableWin.pdb ?Transfer@CSpecialAbilitiesDrainLifeDef@@UAEXAAVCPersistContext@@@Z, the def's
// override of CDefClassBase::Transfer (virtual slot 18). Declaration only (the elaborated class needs no include); no layout change.
public: virtual void Transfer(class CPersistContext&);
public:
#endif
 std::vector<float> AmountOfHealthSucked;
 std::vector<long> NumEffects;
 std::vector<float> SpellRange;
 std::vector<CCharString> ShotName;
 float SpellDelaySecs;
};
FABLE_STATIC_ASSERT_AT(15, offsetof(CSpecialAbilitiesDrainLifeDef,SpellRange)==0x54);
FABLE_STATIC_ASSERT_AT(16, sizeof(CSpecialAbilitiesDrainLifeDef)==0x70);
#endif
#endif
// Retail opt-in (Codex pass22): both original PDBs,all members,actual parent and four12-byte native vectors.
