#pragma optimize("t", on)
// Leaf: `scalar_deleting_destructor'
#define CScriptGameResourceObjectScriptedThingBase CScriptGameResourceObjectScriptedThingBasePdb
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#undef CScriptGameResourceObjectScriptedThingBase
class CScriptGameResourceObjectScriptedThingBase { public:   // ctor 0x007E72A0 (Ego_r.pdb; the member's own model)
    virtual ~CScriptGameResourceObjectScriptedThingBase();   // 0x007E74D0
};
extern void __fastcall AutoTinyCompositeDestructor_00f35b40(void* self);
void* __fastcall CEngineSubPrimitiveText_CompositeScalarDeletingDestructor(void* self, int, unsigned int flags)
{
    reinterpret_cast<CScriptGameResourceObjectScriptedThingBase*>(static_cast<unsigned char*>(self) + 36)->CScriptGameResourceObjectScriptedThingBase::~CScriptGameResourceObjectScriptedThingBase();
    AutoTinyCompositeDestructor_00f35b40(self);
    if (flags & 1)
        operator delete(self);
    return self;
}