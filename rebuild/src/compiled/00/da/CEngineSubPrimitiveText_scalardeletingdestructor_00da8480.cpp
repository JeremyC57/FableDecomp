#pragma optimize("t", on)
// Leaf: `scalar_deleting_destructor'
// Naming wave 28 (family sfb): the base destructor is ??1CSpawnedFuncBase@@UAE@XZ 0x00CDD4C0 (chunk 696: the ctor 0x00CDD450 stores vtable
//   0x012C2F9C, whose ??_G 0x00CDD4A0 calls it; Ego_r.pdb + FableWin.pdb), called qualified. Was the free
//   __fastcall ?AutoTinyCompositeDestructor@@YIXPAX@Z stand-in.
class CCPPointerInfo;
#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
class CSpawnedFuncBase { public: virtual ~CSpawnedFuncBase(); };   // 0x00CDD4C0 (vtable 0x012C2F9C)
void* __fastcall CEngineSubPrimitiveText_CompositeScalarDeletingDestructor(void* self, int, unsigned int flags)
{
    reinterpret_cast<CScriptThing*>(static_cast<unsigned char*>(self) + 60)->CScriptThing::~CScriptThing();
    ((CSpawnedFuncBase*)(self))->CSpawnedFuncBase::~CSpawnedFuncBase();
    if (flags & 1)
        operator delete(self);
    return self;
}