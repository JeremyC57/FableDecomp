#ifndef FABLE_ENGINE_CTCHEALLIFE_H
#define FABLE_ENGINE_CTCHEALLIFE_H
// Both original PDBs: native containers reduce original size64 to retail48.
#include "engine/CTCBase.h"
#include "engine/CDefPointer.h"
#include "engine/CIntelligentPointer.h"
#include "stlport/fable_containers.h"
class CSpecialAbilitiesHealLifeDef;class CThingCreatureBase;
class CTCHealLife:public CTCBase {
public:
#ifdef FABLE_RETAIL_CTCHEALLIFE_CTOR_PASS23
 // Both original PDB PUBLICs: original CThing& constructor.
public: CTCHealLife(CThing&);
#endif
 void StartHealingEffect();
#ifdef FABLE_RETAIL_CTCHEALLIFE_FRAMEUPDATE_PASS26
 // Ego_r 0x007C2D30 / FableWin 0x02713BD0; retail vtable slot 0x0126E390.
 virtual void FrameUpdate();
#endif
private:
 CCharString GetLevelEffectName();
 CDefPointer<const CSpecialAbilitiesHealLifeDef> PDef;
 float TotalHealthAccum;
 unsigned long LoopingSound;
 long SpellLevel;
 CIntelligentPointer<CThing> PItemRemoved;
 std::list<CIntelligentPointer<CThingCreatureBase> > ThingsToHeal;
 std::vector<CIntelligentPointer<CThing> > HealingEffects;
 std::vector<float> HealthIncreaseAccum;
 CIntelligentPointer<CThing> PHealingTarget;
};
FABLE_STATIC_ASSERT_AT(24, sizeof(CTCHealLife)==0x48);
#endif
