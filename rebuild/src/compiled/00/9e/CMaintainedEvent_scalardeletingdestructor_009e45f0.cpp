#pragma optimize("t", on)
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
extern void __fastcall DestructMember_00a03b30(void* member);
void* __fastcall CMaintainedEvent_CompositeScalarDeletingDestructor(void* event, int, unsigned int flags)
{
    DestructMember_00a03b30(static_cast<unsigned char*>(event) + 4);
    ((CBaseClass*)(event))->CBaseClass::~CBaseClass();
    if (flags & 1)
        operator delete(event);
    return event;
}