#pragma optimize("t", on)
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CCharString { public:
    ~CCharString();   // 0x0099EAE0 (Ego_r.pdb ??1CCharString@@QAE@XZ)
};
void* __fastcall CActionEventRemoveObject_CompositeScalarDeletingDestructor(void* action, int, unsigned int flags)
{
    ((CCharString*)(static_cast<unsigned char*>(action) + 20))->~CCharString();
    ((CBaseClass*)(action))->CBaseClass::~CBaseClass();
    if (flags & 1)
        operator delete(action);
    return action;
}