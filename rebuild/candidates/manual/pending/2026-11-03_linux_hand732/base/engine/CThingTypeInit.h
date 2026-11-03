#ifndef FABLE_ENGINE_CTHINGTYPEINIT_H
#define FABLE_ENGINE_CTHINGTYPEINIT_H
#include "engine/CClassTypeInit.h"
// Original public base and original fields; native stack offsets +10/+14.
class CThingTypeInit:public CClassTypeInit {
public:
    unsigned long ThingTypeFlags;
    CCharString DefClassType;
    CThingTypeInit(long type,const CCharString& def,unsigned long flags,CClass*(__fastcall *alloc)(),bool local,const CCharString& name)
      :CClassTypeInit(type,alloc,local,name),ThingTypeFlags(flags),DefClassType(def) {}
};
FABLE_STATIC_ASSERT(sizeof(CThingTypeInit)==24);
#endif
