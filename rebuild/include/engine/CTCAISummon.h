#ifndef FABLE_ENGINE_CTCAISUMMON_H
#define FABLE_ENGINE_CTCAISUMMON_H
#include "engine/CTCBase.h"
#include "engine/CDefPointer.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CSummonDef.h"
#include "stlport/fable_containers.h"
class CTCAISummon : public CTCBase {
    static std::list<CIntelligentPointer<CThing> > AllSummonedCreatures;
    CDefPointer<const CSummonDef> PSummonDef;
    std::list<CIntelligentPointer<CThing> > MySummonedCreatures;
    long LastSummonTick;
public:
    const CSummonDef& GetSummonDef() const { return *PSummonDef.Get(); }
    long GetNumberCanSummon() const;
    long SummonCreatures(const C3DVector* summonCentre);
};
FABLE_STATIC_ASSERT(sizeof(CTCAISummon)==0x18);
#endif
