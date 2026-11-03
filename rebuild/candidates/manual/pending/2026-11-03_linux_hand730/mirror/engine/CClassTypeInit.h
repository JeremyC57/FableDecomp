#ifndef FABLE_ENGINE_CCLASSTYPEINIT_H
#define FABLE_ENGINE_CCLASSTYPEINIT_H
#include "fable_string.h"
class CClass;
// Original Ego_r TPI CClassTypeInit; native registration frame offsets 0,4,8,c.
class CClassTypeInit {
public:
    unsigned char TypeID;
    CClass* (__fastcall *AllocFunc)();
    bool Local;
    CCharString Name;
    CClassTypeInit(long type,CClass*(__fastcall *alloc)(),bool local,const CCharString& name)
      : TypeID((unsigned char)type),AllocFunc(alloc),Local(local),Name(name) {}
};
FABLE_STATIC_ASSERT(sizeof(CClassTypeInit)==16);
#endif
