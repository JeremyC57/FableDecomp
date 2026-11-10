#ifndef FABLE_ENGINE_CBOSSDEF_H
#define FABLE_ENGINE_CBOSSDEF_H
#include "engine/CSubDefClassBase.h"
#include "stlport/fable_containers.h"
// Full original Ego_r0x2fa5b members; native three STLport vectors12 rather than16.
class CBossDef:public CSubDefClassBase {
public:
 virtual void Copy(const CDefClassBase*);
 virtual unsigned long GetSizeofClass();
 virtual void Transfer(CPersistContext&);
 static CDefClassBase* FABLE_FASTCALL Alloc();
 static CCharString FABLE_FASTCALL StatGetTypeName();
 std::vector<long> PhaseHealthPercent,Shots,TwinBladeMissesBeforeLunge;
 long TwinBladeStuckLoops;
 float TwinBladeTimeBeforeLunge;
};
FABLE_STATIC_ASSERT(sizeof(CBossDef)==84);
#endif
