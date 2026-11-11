#ifndef FABLE_ENGINE_BASICPARTICLEANDENDPOINTS_H
#define FABLE_ENGINE_BASICPARTICLEANDENDPOINTS_H
#include "fable_string.h"
// Layout: complete original Ego_r/FableWin member records; native STLport containers are twelve bytes.
struct BasicParticleAndEndPoints{
public:
 CCharString StartPoint;
 CCharString EndPoint;
 long ParticleEffectID;
 long ParticleEffectIDToBlendTo;
};
FABLE_STATIC_ASSERT(sizeof(BasicParticleAndEndPoints)==16);
#endif
