#ifndef FABLE_ENGINE_CENGINESUBPRIMITIVESPRITETRAIL_H
#define FABLE_ENGINE_CENGINESUBPRIMITIVESPRITETRAIL_H
// Original PDB types and native layout; see 00bca6b0/NAMING_EVIDENCE.md.
// NF86 opt-in FABLE_RETAIL_CENGINESUBPRIMITIVESPRITETRAIL_IMPLICIT_DTOR (round 7, q11117 row 0x00BCA060, its scalar deleting destructor): retail's
// destructor chain is implicit (0x00BCA060 calls ~CBaseClassNonCopyable 0x0099A300 straight, as every sub-primitive ??_G does), so the
// declared destructor is left out; use with FABLE_RETAIL_PASS22_PRIMITIVE_IMPLICIT_DESTRUCTORS. Same layout.
#include "engine/CEngineSubPrimitiveBase.h"
#include "engine/CEngineInternalPrimitiveSpriteTrail.h"
class CEngineSubPrimitiveSpriteTrail:public CEngineSubPrimitiveBase {
public:
 CEngineSubPrimitiveSpriteTrail(CEngineInternalPrimitiveSpriteTrail* parent):CEngineSubPrimitiveBase(parent){}
#ifndef FABLE_RETAIL_CENGINESUBPRIMITIVESPRITETRAIL_IMPLICIT_DTOR
 virtual ~CEngineSubPrimitiveSpriteTrail();
#endif
 const CEngineInternalPrimitiveSpriteTrail* PeekParent()const;
 CEngineInternalPrimitiveSpriteTrail* GetParent();
};
FABLE_STATIC_ASSERT_AT(13, sizeof(CEngineSubPrimitiveSpriteTrail)==24);
#endif
