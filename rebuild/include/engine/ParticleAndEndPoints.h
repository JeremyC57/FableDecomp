#ifndef FABLE_ENGINE_PARTICLEANDENDPOINTS_H
#define FABLE_ENGINE_PARTICLEANDENDPOINTS_H
#include "engine/BasicParticleAndEndPoints.h"
// Layout: complete original Ego_r/FableWin member records; native STLport containers are twelve bytes.
struct ParticleAndEndPoints:public BasicParticleAndEndPoints{
public:
 long PeriodicFrames;
};
FABLE_STATIC_ASSERT(sizeof(ParticleAndEndPoints)==20);
#endif
