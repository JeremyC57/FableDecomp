#ifndef FABLE_ENGINE_CENGINESUBPRIMITIVESPRITETRAIL_H
#define FABLE_ENGINE_CENGINESUBPRIMITIVESPRITETRAIL_H
// Original PDB types and native layout; see 00bca6b0/NAMING_EVIDENCE.md.
#include "engine/CEngineSubPrimitiveBase.h"
#include "engine/CEngineInternalPrimitiveSpriteTrail.h"
class CEngineSubPrimitiveSpriteTrail:public CEngineSubPrimitiveBase {
public:
 CEngineSubPrimitiveSpriteTrail(CEngineInternalPrimitiveSpriteTrail* parent):CEngineSubPrimitiveBase(parent){}
 virtual ~CEngineSubPrimitiveSpriteTrail();
 const CEngineInternalPrimitiveSpriteTrail* PeekParent()const;
 CEngineInternalPrimitiveSpriteTrail* GetParent();
};
FABLE_STATIC_ASSERT(sizeof(CEngineSubPrimitiveSpriteTrail)==24);
#endif
