#ifndef FABLE_ENGINE_CENGINEPRIMITIVEPARTICLESPRITE_H
#define FABLE_ENGINE_CENGINEPRIMITIVEPARTICLESPRITE_H
#include "engine/CEnginePrimitive.h"
#include "engine/C3DVector.h"
#include "engine/C2DVector.h"
#include "engine/CRGBColour.h"
// Layout: both original PDB types; native AFCC30 accesses and constructor neighbours.
class CEnginePrimitiveParticleSprite:public CEnginePrimitive{public:
unsigned long Frame;C3DVector WorldPos;C2DVector Size;float Angle;CRGBColour Colour;
CEnginePrimitiveParticleSprite(unsigned __int64,unsigned long,const C3DVector&,const C2DVector&,float,const CRGBColour&,EEngineInterpolationSet);
};
FABLE_STATIC_ASSERT(sizeof(CEnginePrimitiveParticleSprite)==44);
#endif
