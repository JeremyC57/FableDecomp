#ifndef FABLE_ENGINE_COBJECTAUGMENTATIONPARTICLESET_H
#define FABLE_ENGINE_COBJECTAUGMENTATIONPARTICLESET_H
#include "engine/CArray.h"
#include "engine/CDefString.h"
// Layout: complete original Ego_r/FableWin member records; native STLport containers are twelve bytes.
class CObjectAugmentationParticleSet{
public:
 CObjectAugmentationParticleSet(const CObjectAugmentationParticleSet & p0);
 CObjectAugmentationParticleSet();
 float PeriodSecs;
 long ParticleEffect;
 long ParticleEffectToBlendTo;
 long AttackParticleEffect;
 long AttackParticleEffectToBlendTo;
 CArray<CDefString> StartPoint;
 CArray<CDefString> EndPoint;
 ~CObjectAugmentationParticleSet();
 CObjectAugmentationParticleSet & operator=(const CObjectAugmentationParticleSet & p0);
};
FABLE_STATIC_ASSERT(sizeof(CObjectAugmentationParticleSet)==44);
#endif
