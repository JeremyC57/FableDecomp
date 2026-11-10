#ifndef FABLE_ENGINE_CENGINEPRIMITIVELINE_H
#define FABLE_ENGINE_CENGINEPRIMITIVELINE_H
#include "engine/CEnginePrimitive.h"
#include "engine/C3DVector.h"
#include "engine/CRGBColour.h"
enum EEnginePrimitiveBlendMode;
enum EEnginePrimitiveBlendOp;
class CEnginePrimitiveLine:public CEnginePrimitive {
public:
 CEnginePrimitiveLine(unsigned __int64,const C3DVector&,const C3DVector&,const CRGBColour&,float,EEnginePrimitiveBlendMode,unsigned long,bool,EEngineInterpolationSet,EEnginePrimitiveBlendOp);
 C3DVector Start,End;
 CRGBColour Colour;
 float Width;
 unsigned long BankIndex;
 EEnginePrimitiveBlendMode BlendMode;
 EEnginePrimitiveBlendOp BlendOp;
 bool UseZBuffer;
};
FABLE_STATIC_ASSERT(sizeof(CEnginePrimitiveLine)==60);
#endif
