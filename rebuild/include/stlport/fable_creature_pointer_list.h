#ifndef FABLE_STLPORT_CREATURE_POINTER_LIST_H
#define FABLE_STLPORT_CREATURE_POINTER_LIST_H
#include "engine/CThingAICreature.h"
#include "stlport/fable_containers.h"

#if defined(FABLE_RETAIL_STLP_CREATURE_LIST_PUSHBACK_SIZE_INSTANCE_PASS26) && defined(_MSC_VER)
namespace std {
// Fidelity: scaffolding, not retail. 006604EB sets optimize("s", on) and optimize("y", off).
// The opt-in resets with optimize("", on) and needs any row-head pragmas after this include.
#pragma optimize("s", on)
#pragma optimize("y", off)
template<>
inline void list<CIntelligentPointer<CThingAICreature> >::push_back(const CIntelligentPointer<CThingAICreature>& value)
{
    insert(end(), value);
}
#pragma optimize("", on)
}
#endif
#endif
