// CThingSwitch::`vector_deleting_destructor' @ 0x00565870 (retail, 48 bytes) -- a scalar deleting destructor body (`??_G`, flags in the
// Leaf: `vector_deleting_destructor'
// stack slot, __thiscall shape modelled as __fastcall with a dead edx): destroys the members last-to-first,
// +0xc CCharString (~ @ 0x0099EAE0), +0x8 CWideString (~ @ 0x0099B510), then +0x0 CWideString (~ @ 0x0099B510), and frees the block with operator delete
// (MSVCR71 thunk 0x00BFE9BC) when flags & 1. Each callee is declared under its own identity (vtable RTTI /
// retail labels); the class itself is not attributed, so it is named by address.
#pragma optimize("t", on)
struct CWideString {                        // one word: CWideStringData*
    void* PStringData;
    ~CWideString();                         // 0x0099B510 (NoRefs at data+0xc, free(), instance count 0x013BCA20)
};
struct CCharString {                        // one word: CCharStringData*
    void* PStringData;
    ~CCharString();                         // 0x0099EAE0 (release @ 0x0099E9B0, instance count 0x013BD800)
};
void __cdecl operator delete(void* block);  // MSVCR71 import thunk 0x00BFE9BC

#pragma pack(push, 1)
struct CObject_00565870 {
    CWideString Member0;
    unsigned char _pad8[0x4];
    CWideString Member8;                        // +0x8
    CCharString Memberc;                        // +0xc
};
#pragma pack(pop)

void* __fastcall CThingSwitch_CompositeScalarDeletingDestructor(void* self, int, unsigned int flags)
{
    CObject_00565870* object = static_cast<CObject_00565870*>(self);
    object->Memberc.~CCharString();
    object->Member8.~CWideString();
    object->Member0.~CWideString();
    if (flags & 1)
        operator delete(object);
    return self;
}
