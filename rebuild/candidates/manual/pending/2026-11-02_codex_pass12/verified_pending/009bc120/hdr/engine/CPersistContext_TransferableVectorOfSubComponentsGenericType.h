#ifndef FABLE_ENGINE_CPERSISTCONTEXT_TRANSFERABLEVECTOROFSUBCOMPONENTSGENERICTYPE_H
#define FABLE_ENGINE_CPERSISTCONTEXT_TRANSFERABLEVECTOROFSUBCOMPONENTSGENERICTYPE_H
#include "engine/CPersistContext.h"
// Original Ego_r TPIbfef, class V, size4; native009bc120 uses all five slots in this order.
class CPersistContext_TransferableVectorOfSubComponentsGenericType {
public:
    virtual long Size()=0;
    virtual void Resize(long)=0;
    virtual bool AreDefaultValsApplied(long)=0;
    virtual void SetDefaultValsApplied(long,bool)=0;
    virtual void Transfer(CPersistContext&,long)=0;
};
FABLE_STATIC_ASSERT(sizeof(CPersistContext_TransferableVectorOfSubComponentsGenericType)==4);
#endif
