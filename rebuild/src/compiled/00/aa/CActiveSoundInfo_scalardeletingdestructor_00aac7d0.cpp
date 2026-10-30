#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
struct CActiveSoundInfo {
    void* vftable;
};

void operator delete(void*);

void* __fastcall CActiveSoundInfo_scalar_deleting_destructor(CActiveSoundInfo* info, int edx_unused, unsigned int flags)
{
    ((CBaseClass*)(info))->CBaseClass::~CBaseClass();
    if (flags & 1)
        operator delete(info);
    return info;
}