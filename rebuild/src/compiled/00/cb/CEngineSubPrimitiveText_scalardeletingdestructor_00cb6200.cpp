// CEngineSubPrimitiveText::`scalar_deleting_destructor' @ 0x00CB6200 (retail, 40 bytes) -- a scalar deleting destructor body (`??_G`, flags in the
// Leaf: `scalar_deleting_destructor'
// stack slot, __thiscall shape modelled as __fastcall with a dead edx): destroys the members last-to-first,
// +0xc CCharString (~ @ 0x0099EAE0), then +0x0 CCharString (~ @ 0x0099EAE0), and frees the block with operator delete
// (MSVCR71 thunk 0x00BFE9BC) when flags & 1. Each callee is declared under its own identity (vtable RTTI /
// retail labels); the class itself is not attributed, so it is named by address.
#pragma optimize("t", on)
class CCharString { public:                        // one word: CCharStringData*
    void* PStringData;
    ~CCharString();                         // 0x0099EAE0 (release @ 0x0099E9B0, instance count 0x013BD800)
};
void __cdecl operator delete(void* block);  // MSVCR71 import thunk 0x00BFE9BC

#pragma pack(push, 1)
struct CObject_00cb6200 {
    CCharString Member0;
    unsigned char _padc[0x8];
    CCharString Memberc;                        // +0xc
};
#pragma pack(pop)

void* __fastcall CEngineSubPrimitiveText_CompositeScalarDeletingDestructor(void* self, int, unsigned int flags)
{
    CObject_00cb6200* object = static_cast<CObject_00cb6200*>(self);
    object->Memberc.~CCharString();
    object->Member0.~CCharString();
    if (flags & 1)
        operator delete(object);
    return self;
}
