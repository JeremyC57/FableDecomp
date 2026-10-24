// CThingSwitch::`vector_deleting_destructor' @ 0x00E0E7B0 (retail, 48 bytes) -- a scalar deleting destructor body (`??_G`, flags in the
// Leaf: `vector_deleting_destructor'
// stack slot, __thiscall shape modelled as __fastcall with a dead edx): destroys the members last-to-first,
// +0x6c CVector_008ac970 (~ @ 0x008AC970), +0x60 CVector_008ac970 (~ @ 0x008AC970), then the base CRecordSet_00cbd510 (~ @ 0x00CBD510), and frees the block with operator delete
// (MSVCR71 thunk 0x00BFE9BC) when flags & 1. Each callee is declared under its own identity (vtable RTTI /
// retail labels); the class itself is not attributed, so it is named by address.
#pragma optimize("t", on)
struct CRecordSet_00cbd510 {                // catalog/landed name for the class whose destructor is 0x00CBD510
    void* _vptr;                            // (layout beyond the first word not modelled)
    ~CRecordSet_00cbd510();                 // 0x00CBD510
};
struct CVector_008ac970 {                   // unattributed vector of polymorphic elements (slot-0 destroy with flag 0, as vector<CConversation>)
    void* _M_start; void* _M_finish; void* _M_end_of_storage;
    ~CVector_008ac970();                    // 0x008AC970 (class not identified)
};
void __cdecl operator delete(void* block);  // MSVCR71 import thunk 0x00BFE9BC

#pragma pack(push, 1)
struct CObject_00e0e7b0 : CRecordSet_00cbd510 {
    unsigned char _pad60[0x5c];
    CVector_008ac970 Member60;                  // +0x60
    CVector_008ac970 Member6c;                  // +0x6c
};
#pragma pack(pop)

void* __fastcall CThingSwitch_CompositeScalarDeletingDestructor(void* self, int, unsigned int flags)
{
    CObject_00e0e7b0* object = static_cast<CObject_00e0e7b0*>(self);
    object->Member6c.~CVector_008ac970();
    object->Member60.~CVector_008ac970();
    object->CRecordSet_00cbd510::~CRecordSet_00cbd510();
    if (flags & 1)
        operator delete(object);
    return self;
}
