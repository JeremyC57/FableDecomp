#ifndef FABLE_ENGINE_CDEFPOINTEEBASE_H
#define FABLE_ENGINE_CDEFPOINTEEBASE_H
// Retail layout (hand-written; no Ego_r layout row: the class adds no members). RTTI: CDefPointeeBase : CResource.
// Vtable 0x01230BA0: slot 0 scalar deleting dtor 0x0042D9A0, slots 1-9 inherited from CResource unchanged.
// Only the FABLE_RETAIL_DEF_CLASS_BASE opt-in uses it (included by engine/CDefClassBase.h).
#include <stddef.h>
#include "rebuild_abi.h"
#include "engine/CResource.h"

class CDefPointeeBase : public CResource {
public:
    virtual ~CDefPointeeBase();                                          // slot 0  0x0042D9A0 (scalar deleting)
};
FABLE_STATIC_ASSERT(sizeof(CDefPointeeBase) == 0x1c);

#endif // FABLE_ENGINE_CDEFPOINTEEBASE_H
