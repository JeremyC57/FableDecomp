#pragma optimize("t", on)
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CInputEvent { public:
    ~CInputEvent();   // 0x00A03B30 (Ego_r.pdb ??1CInputEvent@@QAE@XZ)
};
void* __fastcall CMaintainedEvent_CompositeScalarDeletingDestructor(void* event, int, unsigned int flags)
{
    ((CInputEvent*)(static_cast<unsigned char*>(event) + 4))->~CInputEvent();
    ((CBaseClass*)(event))->CBaseClass::~CBaseClass();
    if (flags & 1)
        operator delete(event);
    return event;
}