#ifndef FABLE_ENGINE_CPERSISTCONTEXT_TRANSFERABLEVECTOROFSUBCOMPONENTSGENERICTYPE_H
#define FABLE_ENGINE_CPERSISTCONTEXT_TRANSFERABLEVECTOROFSUBCOMPONENTSGENERICTYPE_H
#include "rebuild_abi.h"
class CPersistContext;
// CPersistContext_TransferableVectorOfSubComponentsGenericType (Ego_r TPI, class V, size 4): an interface of five pure
// virtuals; 0x009BC120 calls slots 0..4 in this order (Size +0, Resize +4, AreDefaultValsApplied +8, SetDefaultValsApplied +0xC,
// Transfer +0x10).
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
