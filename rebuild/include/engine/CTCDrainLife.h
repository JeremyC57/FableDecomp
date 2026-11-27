#ifndef FABLE_ENGINE_CTCDRAINLIFE_H
#define FABLE_ENGINE_CTCDRAINLIFE_H
#ifdef FABLE_RETAIL_CTCDRAINLIFE_PASS22
#include "engine/CTCBase.h"
#include "engine/CDefPointer.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CSpecialAbilitiesDrainLifeDef.h"
#include "stlport/fable_containers.h"
class CThingCreatureBase;
class CTCDrainLife:public CTCBase {
public:
 CDefPointer<const CSpecialAbilitiesDrainLifeDef> PDef;
 long SpellLevel;
 CIntelligentPointer<CThing> PItemRemoved;
 std::vector<CIntelligentPointer<CThing> > Shots;
 bool JustReleased;
 long SpellDelayFrames;
 virtual void FrameUpdate();
#ifdef FABLE_RETAIL_DRAIN_PULSE_PASS29
 long GetSoulEffect() const;
#endif
private:
 void DoDrain(CThingCreatureBase*);
};
FABLE_STATIC_ASSERT_AT(22, sizeof(CTCDrainLife)==0x30);
#endif
#endif
// Retail opt-in (Codex pass22): complete original component,retail vector12,original private DoDrain signature.
