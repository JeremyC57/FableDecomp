#ifndef FABLE_ENGINE_CENGINEINTERNALPRIMITIVESPRITETRAIL_H
#define FABLE_ENGINE_CENGINEINTERNALPRIMITIVESPRITETRAIL_H
// Original PDB types and native layout; see 00bca6b0/NAMING_EVIDENCE.md.
#include "engine/CEngineInternalPrimitiveBase.h"
#include "engine/CArray.h"
#include "engine/C3DVector.h"
#include "engine/CRGBColour.h"
#include "engine/CMatrix3x4.h"
#include "engine/CEngineRenderEnums.h"
class CEngineInternalPrimitiveSpriteTrail:public CEngineInternalPrimitiveBase {
public:
 CEngineInternalPrimitiveSpriteTrail();
 virtual ~CEngineInternalPrimitiveSpriteTrail();
 virtual void Update(const CEnginePrimitive&,unsigned long);
 virtual bool GetWorldPosition(C3DVector&);
 class CSpriteTrailEntry {public:C3DVector Position;float Width;};
 unsigned long MaxLength,GraphicIndex,GraphicFrame;
 float SkipPoints,Width;
 CRGBColour HeadColour,MidColour,TailColour;
 CArray<CSpriteTrailEntry> Trail;
 long NextPos;
 unsigned short Flags;
 EEnginePrimitiveBlendOp BlendOp;
 EEnginePrimitiveBlendMode BlendMode;
 CMatrix3x4 WorldMat;
};
FABLE_STATIC_ASSERT(sizeof(CEngineInternalPrimitiveSpriteTrail)==180);
#endif
