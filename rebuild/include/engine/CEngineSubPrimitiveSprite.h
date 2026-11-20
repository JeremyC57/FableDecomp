#ifndef FABLE_ENGINE_CENGINESUBPRIMITIVESPRITE_H
#define FABLE_ENGINE_CENGINESUBPRIMITIVESPRITE_H
// New header (hand3c2, row 0x00BADE40 CEnginePrimitiveManagerSprites::RenderSprite), the twin of engine/CEngineSubPrimitiveSpriteTrail.h:
// RTTI .?AVCEngineSubPrimitiveSprite@@ : CEngineSubPrimitiveBase, vtable 0x012A5430 (stored by the row at 0x00BAE121), sizeof 0x18 (the
// row's `mov ecx,18h ; call operator new` 0x00BAE100); Ego_d.pdb names the row's local `CEngineSubPrimitiveSprite* subprim`. The
// constructor is inline in retail (through the inline CEngineSubPrimitiveBase constructor: CBaseClassNonCopyable constructor 0x0099A2F0,
// Parent +0x0C, NextSubPrimitive +0x10 = 0, then the vtable); Ego_d.pdb ??0CEngineSubPrimitiveSprite is one 37-byte source line.
#include "engine/CEngineSubPrimitiveBase.h"
#include "engine/CEngineInternalPrimitiveSprite.h"
class CEngineSubPrimitiveSprite:public CEngineSubPrimitiveBase {
public:
 CEngineSubPrimitiveSprite(CEngineInternalPrimitiveSprite* parent):CEngineSubPrimitiveBase(parent){}
#ifndef FABLE_RETAIL_PASS22_PRIMITIVE_IMPLICIT_DESTRUCTORS
 virtual ~CEngineSubPrimitiveSprite();
#endif
};
FABLE_STATIC_ASSERT_AT(13, sizeof(CEngineSubPrimitiveSprite)==24);
#endif
