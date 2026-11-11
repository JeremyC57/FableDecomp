#ifndef FABLE_ENGINE_CENGINEPRIMITIVEFLARESPRITE_H
#define FABLE_ENGINE_CENGINEPRIMITIVEFLARESPRITE_H
#include "engine/CEnginePrimitive.h"
#include "engine/C3DVector.h"
#include "engine/CEnginePrimitive2DSprite.h"
// Layout: both original PDBs and donor0222b220,complete size68.
class CEnginePrimitiveFlareSprite:public CEnginePrimitive {
public:
 C3DVector WorldPos;
 unsigned long BankIndex,Frame;
 float LightSourceRadius,SizeX,SizeY,MinScale,FadeStart,Angle;
 EEnginePrimitiveBlendMode BlendMode;
 long Identifier;
 unsigned char Alpha;
 CEnginePrimitiveFlareSprite(unsigned __int64 id,const C3DVector& pos,unsigned long bank,unsigned long frame,float angle,float radius,
  float size_x,float size_y,float min_scale,EEnginePrimitiveBlendMode blend,unsigned long identifier,unsigned char alpha,float fade_start,EEngineInterpolationSet interpolation)
  :CEnginePrimitive((EEnginePrimitiveType)17,id,interpolation),WorldPos(pos),BankIndex(bank),Frame(frame),LightSourceRadius(radius),SizeX(size_x),SizeY(size_y),MinScale(min_scale),FadeStart(fade_start),Angle(angle),BlendMode(blend),Identifier(identifier),Alpha(alpha){}
};
FABLE_STATIC_ASSERT(sizeof(CEnginePrimitiveFlareSprite)==68);
#endif
