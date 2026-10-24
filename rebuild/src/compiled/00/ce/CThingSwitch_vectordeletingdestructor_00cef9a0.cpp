// CThingSwitch::`vector_deleting_destructor' @ 0x00CEF9A0 (retail, 48 bytes) -- a scalar deleting destructor body (`??_G`, flags in the
// Leaf: `vector_deleting_destructor'
// stack slot, __thiscall shape modelled as __fastcall with a dead edx): destroys the members last-to-first,
// +0x58 CCharStringVector (~ @ 0x00414EA0), +0x4c CCharStringVector (~ @ 0x00414EA0), then the base CRecordSet_00cbd510 (~ @ 0x00CBD510), and frees the block with operator delete
// (MSVCR71 thunk 0x00BFE9BC) when flags & 1. Each callee is declared under its own identity (vtable RTTI /
// retail labels); the class itself is not attributed, so it is named by address.
#pragma optimize("t", on)
struct CRecordSet_00cbd510 {                // catalog/landed name for the class whose destructor is 0x00CBD510
    void* _vptr;                            // (layout beyond the first word not modelled)
    ~CRecordSet_00cbd510();                 // 0x00CBD510
};
struct CCharStringVector {                  // std::vector<CCharString>: {first, last, end of storage}
    void* _M_start; void* _M_finish; void* _M_end_of_storage;
    ~CCharStringVector();                   // 0x00414EA0 (std::vector<CCharString>::~vector, as landed rows name it)
};
void __cdecl operator delete(void* block);  // MSVCR71 import thunk 0x00BFE9BC

#pragma pack(push, 1)
struct CObject_00cef9a0 : CRecordSet_00cbd510 {
    unsigned char _pad4c[0x48];
    CCharStringVector Member4c;                 // +0x4c
    CCharStringVector Member58;                 // +0x58
};
#pragma pack(pop)

void* __fastcall CThingSwitch_CompositeScalarDeletingDestructor(void* self, int, unsigned int flags)
{
    CObject_00cef9a0* object = static_cast<CObject_00cef9a0*>(self);
    object->Member58.~CCharStringVector();
    object->Member4c.~CCharStringVector();
    object->CRecordSet_00cbd510::~CRecordSet_00cbd510();
    if (flags & 1)
        operator delete(object);
    return self;
}
