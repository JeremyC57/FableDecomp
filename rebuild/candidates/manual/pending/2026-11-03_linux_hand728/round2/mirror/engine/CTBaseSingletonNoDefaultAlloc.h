#ifndef FABLE_ENGINE_CTBASESINGLETONNODEFAULTALLOC_H
#define FABLE_ENGINE_CTBASESINGLETONNODEFAULTALLOC_H
#include "engine/CCountedPointer.h"
// Original PDB template: independent polymorphic class, size4; PClass private static.
template<class T> class CTBaseSingletonNoDefaultAlloc {
private: static CCountedPointer<T> PClass;
public:
    virtual ~CTBaseSingletonNoDefaultAlloc();
    static T& __fastcall Get() { return *PClass; }
};

#endif
