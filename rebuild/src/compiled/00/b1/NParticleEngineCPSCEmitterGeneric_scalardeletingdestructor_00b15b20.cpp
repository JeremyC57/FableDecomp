#pragma optimize("t", on)
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
extern void __fastcall DestructFirstMember_0075f2b0(void* member);
extern void __fastcall CPSCEmitterGeneric_Destruct(void* emitter);
void* __fastcall CPSCEmitterGeneric_CompositeScalarDeletingDestructor(void* emitter, int, unsigned int flags)
{
    DestructFirstMember_0075f2b0(static_cast<unsigned char*>(emitter) + 24);
    ((CBaseClass*)(static_cast<unsigned char*>(emitter) + 12))->CBaseClass::~CBaseClass();
    CPSCEmitterGeneric_Destruct(emitter);
    if (flags & 1)
        operator delete(emitter);
    return emitter;
}