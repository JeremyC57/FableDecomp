// C3DMeshStripifier::`scalar deleting destructor' @ 0x00A8E040 (retail, 48 bytes) -- a scalar deleting destructor body (`??_G`, flags in the
// stack slot, __thiscall shape modelled as __fastcall with a dead edx): destroys the members last-to-first,
// +0xc CCountedPointer_00a8e000 (~ @ 0x00A8E000), +0x4 CCountedPointer_00a8e000 (~ @ 0x00A8E000), then the base CBaseClassNonCopyable (~ @ 0x0099A300), and frees the block with operator delete
// (MSVCR71 thunk 0x00BFE9BC) when flags & 1. Each callee is declared under its own identity (vtable RTTI /
// retail labels); the class itself is not attributed, so it is named by address.
#pragma optimize("t", on)
struct CBaseClassNonCopyable {              // vtable 0x0129A7C4 (.?AVCBaseClassNonCopyable@@); ctor 0x0099A2F0
    void* _vptr;
    ~CBaseClassNonCopyable();               // 0x0099A300 (re-points to the CBaseClass vtable)
};
struct CCountedPointer_00a8e000 {           // counted pointer: control block {count, release fn, object} at +4
    void* _vptr;
    void* PCount;
    ~CCountedPointer_00a8e000();            // 0x00A8E000 (label: the _Dest_val of CCountedPointer<COptimisedPrimitive>; element type not proven)
};
void __cdecl operator delete(void* block);  // MSVCR71 import thunk 0x00BFE9BC

#pragma pack(push, 1)
struct CObject_00a8e040 : CBaseClassNonCopyable {
    CCountedPointer_00a8e000 Member4;           // +0x4
    CCountedPointer_00a8e000 Memberc;           // +0xc
};
#pragma pack(pop)

void* __fastcall C3DMeshStripifier_CompositeScalarDeletingDestructor(void* stripifier, int, unsigned int flags)
{
    CObject_00a8e040* object = static_cast<CObject_00a8e040*>(stripifier);
    object->Memberc.~CCountedPointer_00a8e000();
    object->Member4.~CCountedPointer_00a8e000();
    object->CBaseClassNonCopyable::~CBaseClassNonCopyable();
    if (flags & 1)
        operator delete(object);
    return stripifier;
}
