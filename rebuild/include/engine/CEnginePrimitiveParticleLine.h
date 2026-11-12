#ifndef FABLE_ENGINE_CENGINEPRIMITIVEPARTICLELINE_H
#define FABLE_ENGINE_CENGINEPRIMITIVEPARTICLELINE_H
#include "engine/CEnginePrimitive.h"
#include "engine/C3DVector.h"
#include "engine/C2DVector.h"
#include "engine/CRGBColour.h"
// Layout: both original PDB types; native AFCC30 accesses and constructor neighbours.
class CEnginePrimitiveParticleLine:public CEnginePrimitive{public:
C3DVector Start,End;CRGBColour Colour;float Width;
CEnginePrimitiveParticleLine(unsigned __int64,const C3DVector&,const C3DVector&,const CRGBColour&,float,EEngineInterpolationSet);
};
FABLE_STATIC_ASSERT(sizeof(CEnginePrimitiveParticleLine)==44);
#endif
