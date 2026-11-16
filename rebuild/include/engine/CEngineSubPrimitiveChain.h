#ifndef FABLE_ENGINE_CENGINESUBPRIMITIVECHAIN_H
#define FABLE_ENGINE_CENGINESUBPRIMITIVECHAIN_H
#include "rebuild_abi.h"
class CEngineSubPrimitiveBase;
// Both original PDB types: public two-pointer chain and user empty-chain constructor.
class CEngineSubPrimitiveChain {
public:
 CEngineSubPrimitiveBase* LastOpaque;
 CEngineSubPrimitiveBase* LastAlpha;
 CEngineSubPrimitiveChain():LastOpaque(0),LastAlpha(0){}
};
FABLE_STATIC_ASSERT(sizeof(CEngineSubPrimitiveChain)==8);
#endif
