// CThingSwitch::`vector_deleting_destructor' @ 0x007811D0 (retail, 48 bytes) -- a scalar deleting destructor body (`??_G`, flags in the
// stack slot, __thiscall shape modelled as __fastcall with a dead edx): destroys the members last-to-first,
// +0x24 CBaseIntelligentPointer (~ @ 0x00A01C10), +0x1c CBaseIntelligentPointer (~ @ 0x00A01C10), then the base CCharString (~ @ 0x0099EAE0), and frees the block with operator delete
// (MSVCR71 thunk 0x00BFE9BC) when flags & 1. Each callee is declared under its own identity (vtable RTTI /
// retail labels); the class itself is not attributed, so it is named by address.
#pragma optimize("t", on)
struct CCharString {                        // one word: CCharStringData*
    void* PStringData;
    ~CCharString();                         // 0x0099EAE0 (release @ 0x0099E9B0, instance count 0x013BD800)
};
struct CBaseIntelligentPointer {            // vptr + data (8 bytes); vtable 0x0129C95C
    void* _vptr;
    void* PData;
    ~CBaseIntelligentPointer();             // 0x00A01C10
};
void __cdecl operator delete(void* block);  // MSVCR71 import thunk 0x00BFE9BC

#pragma pack(push, 1)
struct CObject_007811d0 : CCharString {
    unsigned char _pad1c[0x18];
    CBaseIntelligentPointer Member1c;           // +0x1c
    CBaseIntelligentPointer Member24;           // +0x24
};
#pragma pack(pop)

void* __fastcall CThingSwitch_CompositeScalarDeletingDestructor(void* self, int, unsigned int flags)
{
    CObject_007811d0* object = static_cast<CObject_007811d0*>(self);
    object->Member24.~CBaseIntelligentPointer();
    object->Member1c.~CBaseIntelligentPointer();
    object->CCharString::~CCharString();
    if (flags & 1)
        operator delete(object);
    return self;
}
