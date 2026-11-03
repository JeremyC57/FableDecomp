#ifndef FABLE_ENGINE_CFAILEDALLOCATIONHANDLER_H
#define FABLE_ENGINE_CFAILEDALLOCATIONHANDLER_H
#include "rebuild_abi.h"
// Ego_r TPI0xb5fe; retail vtable0129b874.
class CFailedAllocationHandler {
public:
    virtual ~CFailedAllocationHandler();
    virtual bool HandleFailedAllocation(unsigned long)=0;
};
FABLE_STATIC_ASSERT(sizeof(CFailedAllocationHandler)==4);
#endif
