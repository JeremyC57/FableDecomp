#pragma optimize("s",on)
// scalar deleting destructor for CEngineSubPrimitiveText @ 00456d43
// idiom: push esi; mov esi,ecx; lea ecx,[esi+0x3c]; call member-dtor;
//        mov ecx,esi; call base/self dtor; test [esp+8],1; je; delete this; ret 4

// Layout: CParentDefClassBase is the retail def chain from the shared headers (FABLE_RETAIL_DEF_CLASS_BASE opt-in:
//   vtable 0x01230CBC, 0x39 bytes packed; ctor 0x00430370, ~CParentDefClassBase 0x00430300, copy 0x0046EE98,
//   operator= 0x00431ED0).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CParentDefClassBase.h"
extern void __fastcall MemberAt3C_dtor(void* p);   // dtor of member subobject at +0x3c

void* __fastcall COpinionDeedMaskDef_scalar_deleting_destructor(char* def, int /*edx*/, unsigned int flags)
{
    MemberAt3C_dtor(def + 0x3c);
    ((CParentDefClassBase*)(def))->CParentDefClassBase::~CParentDefClassBase();
    if (flags & 1)
        operator delete(def);
    return def;
}