#ifndef FABLE_ENGINE_CTCANIMATIONBASE_H
#define FABLE_ENGINE_CTCANIMATIONBASE_H
#include "engine/CTCBase.h"
#include "engine/ETCInterfaceType.h"
#include "fable_compiler.h"
// Original empty12-byte derived base, both PDBs and native interface90 tests.
class C3DAnimationBlendState;
class CTCAnimationBase : public CTCBase {
public:
 static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType(){return TCI_ANIMATION;}
 virtual ETCInterfaceType GetInterfaceType()const;
 virtual void GetInterpolatedMeshAnim(C3DAnimationBlendState&,float)const=0;
 virtual void GetMeshAnims(C3DAnimationBlendState&,C3DAnimationBlendState&)const=0;
 virtual bool AnimsCanBeIgnored()const;
 virtual ~CTCAnimationBase();
};
FABLE_STATIC_ASSERT(sizeof(CTCAnimationBase)==12);
#endif
