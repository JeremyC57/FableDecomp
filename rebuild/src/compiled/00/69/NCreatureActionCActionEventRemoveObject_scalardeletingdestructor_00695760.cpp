#pragma optimize("t", on)
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
extern void __fastcall DestructMember_0099eae0(void* member);
void* __fastcall CActionEventRemoveObject_CompositeScalarDeletingDestructor(void* action, int, unsigned int flags)
{
    DestructMember_0099eae0(static_cast<unsigned char*>(action) + 20);
    ((CBaseClass*)(action))->CBaseClass::~CBaseClass();
    if (flags & 1)
        operator delete(action);
    return action;
}