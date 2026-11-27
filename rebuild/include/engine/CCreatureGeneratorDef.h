#ifndef FABLE_ENGINE_CCREATUREGENERATORDEF_H
#define FABLE_ENGINE_CCREATUREGENERATORDEF_H
#include "engine/CSubDefClassBase.h"
#include "engine/CDefString.h"
#include "stlport/fable_containers.h"
class CCreatureGeneratorDef : public CSubDefClassBase {
public:
    std::vector<long> RandomEffects;
    CDefString RandomSoundCriteria;
    long SecondsBetweenEffects;
    long SecondsBetweenSoundEffects;
};
FABLE_STATIC_ASSERT(sizeof(CCreatureGeneratorDef)==0x40);
#endif
