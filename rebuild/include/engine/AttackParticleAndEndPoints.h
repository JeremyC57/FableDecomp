#ifndef FABLE_ENGINE_ATTACKPARTICLEANDENDPOINTS_H
#define FABLE_ENGINE_ATTACKPARTICLEANDENDPOINTS_H
#include "engine/BasicParticleAndEndPoints.h"
#include "engine/CIntelligentPointer.h"
class CThing;
// Layout: complete original Ego_r/FableWin member records; native STLport containers are twelve bytes.
struct AttackParticleAndEndPoints:public BasicParticleAndEndPoints{
public:
 CIntelligentPointer<CThing> ParticleEffect;
};
FABLE_STATIC_ASSERT(sizeof(AttackParticleAndEndPoints)==24);
#endif
