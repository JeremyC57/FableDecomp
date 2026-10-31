#pragma optimize("t", on)
// Leaf: `vector_deleting_destructor'
extern void __fastcall AutoTinyFirstMemberDestructor_00cd4470(void* member);
#define CScriptGameResourceObjectScriptedThingBase CScriptGameResourceObjectScriptedThingBasePdb
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#undef CScriptGameResourceObjectScriptedThingBase
class CScriptGameResourceObjectScriptedThingBase { public:   // ctor 0x007E72A0 (Ego_r.pdb; the member's own model)
    virtual ~CScriptGameResourceObjectScriptedThingBase();   // 0x007E74D0
};
extern void __fastcall AutoTinyCompositeDestructor_00f35b40(void* self);
void* __fastcall CThingSwitch_CompositeScalarDeletingDestructor(void* self, int, unsigned int flags)
{
    AutoTinyFirstMemberDestructor_00cd4470(static_cast<unsigned char*>(self) + 44);
    reinterpret_cast<CScriptGameResourceObjectScriptedThingBase*>(static_cast<unsigned char*>(self) + 28)->CScriptGameResourceObjectScriptedThingBase::~CScriptGameResourceObjectScriptedThingBase();
    AutoTinyCompositeDestructor_00f35b40(self);
    if (flags & 1)
        operator delete(self);
    return self;
}