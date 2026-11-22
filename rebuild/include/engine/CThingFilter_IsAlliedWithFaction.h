#ifndef FABLE_ENGINE_CTHINGFILTER_ISALLIEDWITHFACTION_H
#define FABLE_ENGINE_CTHINGFILTER_ISALLIEDWITHFACTION_H

#include "stlport/fable_containers.h"

class CFaction;
class CThing;

// Original CFaction pointer in both type records and the FableWin constructor at 0x01F27D20.
class CThingFilter_IsAlliedWithFaction : public std::unary_function<CThing, bool> {
public:
    CThingFilter_IsAlliedWithFaction(const CFaction* faction) : PFaction(faction) {}
    bool operator()(const CThing& thing) const; // 0x006C8C50
private:
    const CFaction* PFaction;
};

FABLE_STATIC_ASSERT(sizeof(CThingFilter_IsAlliedWithFaction) == 4);
#endif
