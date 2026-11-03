#ifndef FABLE_ENGINE_CTHINGTYPEINFO_H
#define FABLE_ENGINE_CTHINGTYPEINFO_H
#include "engine/CCharString.h"
// Original PDB class and field order; native stride12, stores +4/+8.
class CThingTypeInfo {
public:
    long DefType;
    unsigned long ThingTypeFlags;
    CCharString DefClassType;
};
FABLE_STATIC_ASSERT(sizeof(CThingTypeInfo)==12);
#endif
