// CEngineSubPrimitiveText::`scalar_deleting_destructor' @ 0x0073D770 (retail, 40 bytes) -- a scalar deleting destructor body (`??_G`, flags in the
// Leaf: `scalar_deleting_destructor'
// stack slot, __thiscall shape modelled as __fastcall with a dead edx): destroys the members last-to-first,
// +0x8 CBaseIntelligentPointer (~ @ 0x00A01C10), then +0x0 CBaseIntelligentPointer (~ @ 0x00A01C10), and frees the block with operator delete
// (MSVCR71 thunk 0x00BFE9BC) when flags & 1. Each callee is declared under its own identity (vtable RTTI /
// retail labels); the class itself is not attributed, so it is named by address.
#pragma optimize("t", on)
struct CBaseIntelligentPointer {            // vptr + data (8 bytes); vtable 0x0129C95C
    void* _vptr;
    void* PData;
    ~CBaseIntelligentPointer();             // 0x00A01C10
};
void __cdecl operator delete(void* block);  // MSVCR71 import thunk 0x00BFE9BC

#pragma pack(push, 1)
struct CObject_0073d770 {
    CBaseIntelligentPointer Member0;
    CBaseIntelligentPointer Member8;            // +0x8
};
#pragma pack(pop)

void* __fastcall CEngineSubPrimitiveText_CompositeScalarDeletingDestructor(void* self, int, unsigned int flags)
{
    CObject_0073d770* object = static_cast<CObject_0073d770*>(self);
    object->Member8.~CBaseIntelligentPointer();
    object->Member0.~CBaseIntelligentPointer();
    if (flags & 1)
        operator delete(object);
    return self;
}
