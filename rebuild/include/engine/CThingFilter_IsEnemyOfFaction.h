#ifndef FABLE_ENGINE_CTHINGFILTER_ISENEMYOFFACTION_H
#define FABLE_ENGINE_CTHINGFILTER_ISENEMYOFFACTION_H
#ifdef FABLE_RETAIL_DEED_WITNESS_PASS28
#include "stlport/fable_containers.h"
class CThing;
class CFaction;
#include "engine/EOpinionDeedType.h"
// Layout: Ego_r member table and the FableWin witness-filter constructor.
class CThingFilter_IsEnemyOfFaction : public std::unary_function<CThing, bool> {
    const CFaction* PFaction;
};
#endif
#endif
