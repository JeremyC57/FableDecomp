// CThingSwitch::`vector_deleting_destructor' @ 0x00E9FBA0 (retail, 48 bytes) -- a scalar deleting destructor body (`??_G`, flags in the
// Leaf: `vector_deleting_destructor'
// stack slot, __thiscall shape modelled as __fastcall with a dead edx): destroys the members last-to-first,
// +0x68 CTimer (~ @ 0x00CD4470), +0x64 CTimer (~ @ 0x00CD4470), then the base CRecordSet_00cbd510 (~ @ 0x00CBD510), and frees the block with operator delete
// (MSVCR71 thunk 0x00BFE9BC) when flags & 1. Each callee is declared under its own identity (vtable RTTI /
// retail labels); the class itself is not attributed, so it is named by address.
#pragma optimize("t", on)
struct CRecordSet_00cbd510 {                // catalog/landed name for the class whose destructor is 0x00CBD510
    void* _vptr;                            // (layout beyond the first word not modelled)
    ~CRecordSet_00cbd510();                 // 0x00CBD510
};
class CTimer { public:                             // one word: the timer handle
    long Handle;
    ~CTimer();                              // 0x00CD4470 (hands the handle back through the global at 0x0143E8F8, slot 0x160)
};
void __cdecl operator delete(void* block);  // MSVCR71 import thunk 0x00BFE9BC

#pragma pack(push, 1)
struct CObject_00e9fba0 : CRecordSet_00cbd510 {
    unsigned char _pad64[0x60];
    CTimer Member64;                            // +0x64
    CTimer Member68;                            // +0x68
};
#pragma pack(pop)

void* __fastcall CThingSwitch_CompositeScalarDeletingDestructor(void* self, int, unsigned int flags)
{
    CObject_00e9fba0* object = static_cast<CObject_00e9fba0*>(self);
    object->Member68.~CTimer();
    object->Member64.~CTimer();
    object->CRecordSet_00cbd510::~CRecordSet_00cbd510();
    if (flags & 1)
        operator delete(object);
    return self;
}
