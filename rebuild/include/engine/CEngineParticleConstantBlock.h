#ifndef FABLE_ENGINE_CENGINEPARTICLECONSTANTBLOCK_H
#define FABLE_ENGINE_CENGINEPARTICLECONSTANTBLOCK_H
// Original shared particle constant record: both PDBs public X/Y/Z/W floats at0/4/8/12, size16.
#include "rebuild_abi.h"
class CEngineParticleConstantBlock {
public:
    float X,Y,Z,W;
};
FABLE_STATIC_ASSERT(sizeof(CEngineParticleConstantBlock)==16);
#endif
