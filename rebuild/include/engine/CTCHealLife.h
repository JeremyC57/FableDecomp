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
 void StartHealingEffect();
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
FABLE_STATIC_ASSERT(sizeof(CTCHealLife)==0x48);
#endif
