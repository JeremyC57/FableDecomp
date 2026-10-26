// NCreatureAction::CActionEventAddObject::`scalar deleting destructor' @ 0x006956E0 (retail, 48 bytes) -- a scalar deleting destructor body (`??_G`, flags in the
// stack slot, __thiscall shape modelled as __fastcall with a dead edx): destroys the members last-to-first,
// +0x18 CCharString (~ @ 0x0099EAE0), +0x14 CCharString (~ @ 0x0099EAE0), then the base CBaseClass (~ @ 0x0099A2E0), and frees the block with operator delete
// (MSVCR71 thunk 0x00BFE9BC) when flags & 1. Each callee is declared under its own identity (vtable RTTI /
// retail labels); the class itself is not attributed, so it is named by address.
#pragma optimize("t", on)
class CBaseClass { public:                         // vtable 0x01231710 (.?AVCBaseClass@@)
    void* _vptr;
    ~CBaseClass();                          // 0x0099A2E0
};
class CCharString { public:                        // one word: CCharStringData*
    void* PStringData;
    ~CCharString();                         // 0x0099EAE0 (release @ 0x0099E9B0, instance count 0x013BD800)
};
void __cdecl operator delete(void* block);  // MSVCR71 import thunk 0x00BFE9BC

#pragma pack(push, 1)
struct CObject_006956e0 : CBaseClass {
    unsigned char _pad14[0x10];
    CCharString Member14;                       // +0x14
    CCharString Member18;                       // +0x18
};
#pragma pack(pop)

void* __fastcall CActionEventAddObject_CompositeScalarDeletingDestructor(void* action, int, unsigned int flags)
{
    CObject_006956e0* object = static_cast<CObject_006956e0*>(action);
    object->Member18.~CCharString();
    object->Member14.~CCharString();
    object->CBaseClass::~CBaseClass();
    if (flags & 1)
        operator delete(object);
    return action;
}
