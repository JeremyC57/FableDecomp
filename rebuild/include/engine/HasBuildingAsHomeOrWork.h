#ifndef FABLE_ENGINE_HASBUILDINGASHOMEORWORK_H
#define FABLE_ENGINE_HASBUILDINGASHOMEORWORK_H
#ifdef FABLE_RETAIL_HASBUILDINGASHOMEORWORK_PASS26
#include "engine/CThingAICreature.h"
#include "engine/CThingBuilding.h"
#include "engine/ThingAlive.h"
// Both original PDBs name the predicate. Its sole field is CThing* Building.
class HasBuildingAsHomeOrWork {
    CThing* Building;
public:
#ifdef FABLE_RETAIL_UPDATE_CURFEW_PASS29
    HasBuildingAsHomeOrWork(const CThing& building):Building(const_cast<CThing*>(&building)){}
#endif
#ifdef FABLE_RETAIL_HASBUILDINGASHOMEORWORK_SIZE_INSTANCE_PASS26
#if defined(FABLE_RETAIL_UPDATE_CURFEW_PASS29) && defined(_MSC_VER)
    __declspec(noinline)
#endif
    bool operator()(const CIntelligentPointer<CThingAICreature>& creature) const;
#else
    bool operator()(const CIntelligentPointer<CThingAICreature>& creature) const
    {
        return IS_THING_ALIVE(creature) &&
               (creature->GetHomeBuilding() == Building || creature->GetWorkBuilding() == Building);
    }
#endif
};
#ifdef FABLE_RETAIL_HASBUILDINGASHOMEORWORK_SIZE_INSTANCE_PASS26
// Fidelity: scaffolding, not retail. The 0065FBE1 owner sets optimize("s", on).
// Reset with optimize("", on). Any row-head optimization must follow this include.
#ifdef _MSC_VER
#pragma optimize("s", on)
#endif
bool HasBuildingAsHomeOrWork::operator()(const CIntelligentPointer<CThingAICreature>& creature) const
{
    if (IS_THING_ALIVE(creature)) {
        if (creature->GetHomeBuilding() == Building || creature->GetWorkBuilding() == Building)
            return true;
    }
    return false;
}
#ifdef _MSC_VER
#pragma optimize("", on)
#endif
#endif
#endif
#endif
