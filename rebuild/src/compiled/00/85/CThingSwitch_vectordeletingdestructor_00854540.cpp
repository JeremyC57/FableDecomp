// CThingSwitch::`vector_deleting_destructor' @ 0x00854540 (retail, 48 bytes) -- a scalar deleting destructor body (`??_G`, flags in the
// stack slot, __thiscall shape modelled as __fastcall with a dead edx): destroys the members last-to-first,
// +0x5c CBaseIntelligentPointer (~ @ 0x00A01C10), +0x54 CBaseIntelligentPointer (~ @ 0x00A01C10), then the base NCameraModes::CScriptModuleBase (~ @ 0x00854310), and frees the block with operator delete
// (MSVCR71 thunk 0x00BFE9BC) when flags & 1. Each callee is declared under its own identity (vtable RTTI /
// retail labels); the class itself is not attributed, so it is named by address.
#pragma optimize("t", on)
namespace NCameraModes {
struct CScriptModuleBase {                 // vtable 0x01255F74 (.?AVCScriptModuleBase@NCameraModes@@)
    void* _vptr;
    ~CScriptModuleBase();                   // 0x00854310
};
}
struct CBaseIntelligentPointer {            // vptr + data (8 bytes); vtable 0x0129C95C
    void* _vptr;
    void* PData;
    ~CBaseIntelligentPointer();             // 0x00A01C10
};
void __cdecl operator delete(void* block);  // MSVCR71 import thunk 0x00BFE9BC

#pragma pack(push, 1)
struct CObject_00854540 : NCameraModes::CScriptModuleBase {
    unsigned char _pad54[0x50];
    CBaseIntelligentPointer Member54;           // +0x54
    CBaseIntelligentPointer Member5c;           // +0x5c
};
#pragma pack(pop)

void* __fastcall CThingSwitch_CompositeScalarDeletingDestructor(void* self, int, unsigned int flags)
{
    CObject_00854540* object = static_cast<CObject_00854540*>(self);
    object->Member5c.~CBaseIntelligentPointer();
    object->Member54.~CBaseIntelligentPointer();
    object->NCameraModes::CScriptModuleBase::~CScriptModuleBase();
    if (flags & 1)
        operator delete(object);
    return self;
}
