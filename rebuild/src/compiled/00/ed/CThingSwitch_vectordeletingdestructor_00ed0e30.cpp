// CThingSwitch::`vector_deleting_destructor' @ 0x00ED0E30 (retail, 48 bytes) -- a scalar deleting destructor body (`??_G`, flags in the
// stack slot, __thiscall shape modelled as __fastcall with a dead edx): destroys the members last-to-first,
// +0x58 CScriptGameResourceObjectScriptedThingBase (~ @ 0x007E74D0), +0x48 CScriptGameResourceObjectScriptedThingBase (~ @ 0x007E74D0), then the base CRecordSet_00cbd510 (~ @ 0x00CBD510), and frees the block with operator delete
// (MSVCR71 thunk 0x00BFE9BC) when flags & 1. Each callee is declared under its own identity (vtable RTTI /
// retail labels); the class itself is not attributed, so it is named by address.
// Engine headers for the callee classes (PDB data layouts, renamed out of the way): the local declarations
// below carry the retail identities the relocations bind.
#define CScriptGameResourceObjectScriptedThingBase CScriptGameResourceObjectScriptedThingBasePdb
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#undef CScriptGameResourceObjectScriptedThingBase
#pragma optimize("t", on)
struct CRecordSet_00cbd510 {                // catalog/landed name for the class whose destructor is 0x00CBD510
    void* _vptr;                            // (layout beyond the first word not modelled)
    ~CRecordSet_00cbd510();                 // 0x00CBD510
};
struct CScriptGameResourceObjectScriptedThingBase {   // ctor 0x007E72A0 (landed rows)
    void* _vptr;                            // (layout beyond the first word not modelled)
    ~CScriptGameResourceObjectScriptedThingBase();   // 0x007E74D0 (releases the counted handle at +0xc)
};
void __cdecl operator delete(void* block);  // MSVCR71 import thunk 0x00BFE9BC

#pragma pack(push, 1)
struct CObject_00ed0e30 : CRecordSet_00cbd510 {
    unsigned char _pad48[0x44];
    CScriptGameResourceObjectScriptedThingBase Member48;// +0x48
    unsigned char _pad58[0xc];
    CScriptGameResourceObjectScriptedThingBase Member58;// +0x58
};
#pragma pack(pop)

void* __fastcall CThingSwitch_CompositeScalarDeletingDestructor(void* self, int, unsigned int flags)
{
    CObject_00ed0e30* object = static_cast<CObject_00ed0e30*>(self);
    object->Member58.~CScriptGameResourceObjectScriptedThingBase();
    object->Member48.~CScriptGameResourceObjectScriptedThingBase();
    object->CRecordSet_00cbd510::~CRecordSet_00cbd510();
    if (flags & 1)
        operator delete(object);
    return self;
}
