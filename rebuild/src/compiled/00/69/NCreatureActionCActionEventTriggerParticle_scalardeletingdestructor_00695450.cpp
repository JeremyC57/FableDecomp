// NCreatureAction::CActionEventTriggerParticle::`scalar deleting destructor' @ 0x00695450 (retail, 48 bytes) -- a scalar deleting destructor body (`??_G`, flags in the
// stack slot, __thiscall shape modelled as __fastcall with a dead edx): destroys the members last-to-first,
// +0x1c CCharString (~ @ 0x0099EAE0), +0x18 CCharString (~ @ 0x0099EAE0), then the base CBaseClass (~ @ 0x0099A2E0), and frees the block with operator delete
// (MSVCR71 thunk 0x00BFE9BC) when flags & 1. Each callee is declared under its own identity (vtable RTTI /
// retail labels); the class itself is not attributed, so it is named by address.
#pragma optimize("t", on)
struct CBaseClass {                         // vtable 0x01231710 (.?AVCBaseClass@@)
    void* _vptr;
    ~CBaseClass();                          // 0x0099A2E0
};
struct CCharString {                        // one word: CCharStringData*
    void* PStringData;
    ~CCharString();                         // 0x0099EAE0 (release @ 0x0099E9B0, instance count 0x013BD800)
};
void __cdecl operator delete(void* block);  // MSVCR71 import thunk 0x00BFE9BC

#pragma pack(push, 1)
struct CObject_00695450 : CBaseClass {
    unsigned char _pad18[0x14];
    CCharString Member18;                       // +0x18
    CCharString Member1c;                       // +0x1c
};
#pragma pack(pop)

void* __fastcall CActionEventTriggerParticle_CompositeScalarDeletingDestructor(void* action, int, unsigned int flags)
{
    CObject_00695450* object = static_cast<CObject_00695450*>(action);
    object->Member1c.~CCharString();
    object->Member18.~CCharString();
    object->CBaseClass::~CBaseClass();
    if (flags & 1)
        operator delete(object);
    return action;
}
