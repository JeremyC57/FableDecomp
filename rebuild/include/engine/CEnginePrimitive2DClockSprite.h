#ifndef FABLE_ENGINE_CENGINEPRIMITIVE2DCLOCKSPRITE_H
#define FABLE_ENGINE_CENGINEPRIMITIVE2DCLOCKSPRITE_H
#include "engine/CEnginePrimitive2DSprite.h"
// Layout: both original PDBs, size96; existing byte-aligned C2DVector places UVs+77,+85.
class CEnginePrimitive2DClockSprite:public CEnginePrimitive {
public:
 C2DBoxF ClipBox;
 EEngine2DLayer Layer;
 unsigned long BankIndex,Frame,BankIndex2,Frame2;
 float AngleStart,AngleEnd;
 EEnginePrimitiveBlendMode BlendMode;
 CRGBColour Colour,Colour2;
 bool ClipBoxIsPos;
 unsigned long Flags;
 bool Filter;
 C2DVector UVOffsetsStart,UVOffsetsEnd;
 // Original FW020dcf04 complete ctor. Its called body exposes genuine input reads.
 CEnginePrimitive2DClockSprite(unsigned __int64 id,const C2DVector&pos,EEngine2DLayer layer,unsigned long bank,unsigned long frame,const CRGBColour&colour,
  unsigned long bank2,unsigned long frame2,const CRGBColour&colour2,EEnginePrimitiveBlendMode blend,float angle_start,float angle_end,
  const C2DVector&uv_start,const C2DVector&uv_end,unsigned long flags,EEngineInterpolationSet interpolation)
 :CEnginePrimitive((EEnginePrimitiveType)35,id,interpolation),ClipBox(pos,pos),Layer(layer),BankIndex(bank),Frame(frame),BankIndex2(bank2),Frame2(frame2),AngleStart(angle_start),AngleEnd(angle_end),BlendMode(blend),Colour(colour),Colour2(colour2),ClipBoxIsPos(true),Flags(flags),Filter(false),UVOffsetsStart(uv_start),UVOffsetsEnd(uv_end){}

};
FABLE_STATIC_ASSERT(sizeof(CEnginePrimitive2DClockSprite)==96);
#endif
