#ifndef FABLE_RETAIL_PASS27_COBJECTFAMILYDEF_H
#define FABLE_RETAIL_PASS27_COBJECTFAMILYDEF_H
#ifdef FABLE_RETAIL_REWARD_PICKUP_PASS27
#include "engine/CParentDefClassBase.h"
class CObjectFamilyEntry {
public:
    long Object;
    long Probability;
};
class CObjectFamilyDef : public CParentDefClassBase {
public:
    long GetRandomObject() const;
private:
    std::vector<CObjectFamilyEntry> Objects;
};
FABLE_STATIC_ASSERT(sizeof(CObjectFamilyDef)==0x48);
#endif
#endif
