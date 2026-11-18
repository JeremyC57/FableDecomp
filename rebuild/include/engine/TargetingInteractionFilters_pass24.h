#ifndef FABLE_ENGINE_TARGETINGINTERACTIONFILTERS_PASS24_H
#define FABLE_ENGINE_TARGETINGINTERACTIONFILTERS_PASS24_H
#ifdef FABLE_RETAIL_TARGETINGINTERACTION_FILTERS_PASS24
#include "rebuild_abi.h"

class CThingFilter_IsAvailableForCreatureInteraction : public std::unary_function<CThingAICreature, bool> {
public:
    CThingFilter_IsAvailableForCreatureInteraction(const CThingAICreature& source, ECreatureInteractionType type)
        : SourceThing(source), Type(type) {}
    bool operator()(const CThingAICreature&) const;                       // 0x006A7430
private:
    const CThingAICreature& SourceThing;
    ECreatureInteractionType Type;
};
class CThingFilter_IsAICreatureAwareOf : public std::unary_function<CThingCreatureBase, bool> {
public:
    explicit CThingFilter_IsAICreatureAwareOf(const CThingAICreature* creature) : PCreature(creature) {}
    bool operator()(const CThingCreatureBase&) const;                     // 0x006A7610
private:
    const CThingAICreature* PCreature;
};
class CThingFilter_IsIncapacitated : public std::unary_function<CThing, bool> {
public:
    bool operator()(const CThing&) const;                                 // 0x006A73F0
};
FABLE_STATIC_ASSERT(sizeof(CThingFilter_IsAvailableForCreatureInteraction) == 8);
FABLE_STATIC_ASSERT(sizeof(CThingFilter_IsAICreatureAwareOf) == 4);
FABLE_STATIC_ASSERT(sizeof(CThingFilter_IsIncapacitated) == 1);
#endif
#endif
