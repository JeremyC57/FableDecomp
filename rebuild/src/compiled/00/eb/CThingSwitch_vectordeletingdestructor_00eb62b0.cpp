#pragma optimize("t", on)
// Leaf: ?CThingSwitch_CompositeScalarDeletingDestructor@@YIPAXPAXHI@Z
// Naming wave 28 (family esb): the base destructor is ??1CEntityScriptBase@@UAE@XZ 0x00F35B40 (chunk 696: stores vtable 0x012C3224, the
//   engine/CEntityScriptBase.h class; Ego_r.pdb + FableWin.pdb), called qualified. Was the free
//   __fastcall ?AutoTinyCompositeDestructor_00f35b40@@YIXPAX@Z stand-in.
class CTimer { public:
    ~CTimer();   // 0x00CD4470 (Ego_r.pdb ??1CTimer@@QAE@XZ)
};
#define CScriptGameResourceObjectScriptedThingBase CScriptGameResourceObjectScriptedThingBasePdb
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#undef CScriptGameResourceObjectScriptedThingBase
class CScriptGameResourceObjectScriptedThingBase { public:   // ctor 0x007E72A0 (Ego_r.pdb; the member's own model)
    virtual ~CScriptGameResourceObjectScriptedThingBase();   // 0x007E74D0
};
class CEntityScriptBase { public: virtual ~CEntityScriptBase(); };   // 0x00F35B40 (vtable 0x012C3224)
void* __fastcall CThingSwitch_CompositeScalarDeletingDestructor(void* self, int, unsigned int flags)
{
    ((CTimer*)(static_cast<unsigned char*>(self) + 44))->~CTimer();
    reinterpret_cast<CScriptGameResourceObjectScriptedThingBase*>(static_cast<unsigned char*>(self) + 28)->CScriptGameResourceObjectScriptedThingBase::~CScriptGameResourceObjectScriptedThingBase();
    ((CEntityScriptBase*)(self))->CEntityScriptBase::~CEntityScriptBase();
    if (flags & 1)
        operator delete(self);
    return self;
}