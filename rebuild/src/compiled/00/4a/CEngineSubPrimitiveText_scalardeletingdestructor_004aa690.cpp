// CEngineSubPrimitiveText::`scalar_deleting_destructor' @ 0x004AA690 (retail, 40 bytes) -- a scalar deleting destructor body (`??_G`, flags in the
// Leaf: `scalar_deleting_destructor'
// stack slot, __thiscall shape modelled as __fastcall with a dead edx): destroys the members last-to-first,
// +0x4 CWideString (~ @ 0x0099B510), then +0x0 CWideString (~ @ 0x0099B510), and frees the block with operator delete
// (MSVCR71 thunk 0x00BFE9BC) when flags & 1. Each callee is declared under its own identity (vtable RTTI /
// retail labels); the class itself is not attributed, so it is named by address.
#pragma optimize("t", on)
class CWideString { public:                        // one word: CWideStringData*
    void* PStringData;
    ~CWideString();                         // 0x0099B510 (NoRefs at data+0xc, free(), instance count 0x013BCA20)
};
void __cdecl operator delete(void* block);  // MSVCR71 import thunk 0x00BFE9BC

#pragma pack(push, 1)
struct CObject_004aa690 {
    CWideString Member0;
    CWideString Member4;                        // +0x4
};
#pragma pack(pop)

void* __fastcall CEngineSubPrimitiveText_CompositeScalarDeletingDestructor(void* self, int, unsigned int flags)
{
    CObject_004aa690* object = static_cast<CObject_004aa690*>(self);
    object->Member4.~CWideString();
    object->Member0.~CWideString();
    if (flags & 1)
        operator delete(object);
    return self;
}
