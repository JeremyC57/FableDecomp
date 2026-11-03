#ifndef FABLE_ENGINE_CCLASSTYPEINFO_H
#define FABLE_ENGINE_CCLASSTYPEINFO_H
#include "fable_string.h"
#include "stlport/fable_containers.h"
class CClass;
// Original PDB fields, retail STLport list4; native ClassTypeInfo stride20.
class CClassTypeInfo {
public:
    std::list<CClass*> TypeList;
    unsigned long NoAllocated;
    bool Used;
    bool Local;
    CClass* (__fastcall *AllocFunc)();
    CCharString Name;
};
FABLE_STATIC_ASSERT(sizeof(CClassTypeInfo)==20);
#endif
