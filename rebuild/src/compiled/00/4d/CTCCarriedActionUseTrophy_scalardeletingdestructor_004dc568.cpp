// CTCCarriedActionUseTrophy::`scalar deleting destructor' @ 0x004DC568 (retail, 38 bytes) -- a scalar deleting destructor body (`??_G`, flags in the
// stack slot, __thiscall shape modelled as __fastcall with a dead edx): destroys the members last-to-first,
// +0x14 C3DAnimationBlendState (~ @ 0x004DA967), then +0x0 C3DAnimationBlendState (~ @ 0x004DA967), and frees the block with operator delete
// (MSVCR71 thunk 0x00BFE9BC) when flags & 1. Each callee is declared under its own identity (vtable RTTI /
// retail labels); the class itself is not attributed, so it is named by address.
#pragma optimize("s", on)
struct C3DAnimationBlendState {             // label C3DAnimationBlendState::~C3DAnimationBlendState
    void* _vptr;                            // (layout beyond the first word not modelled)
    ~C3DAnimationBlendState();              // 0x004DA967 (releases the counted object at +0xc)
};
void __cdecl operator delete(void* block);  // MSVCR71 import thunk 0x00BFE9BC

#pragma pack(push, 1)
struct CObject_004dc568 {
    C3DAnimationBlendState Member0;
    unsigned char _pad14[0x10];
    C3DAnimationBlendState Member14;            // +0x14
};
#pragma pack(pop)

void* __fastcall CTCCarriedActionUseTrophy_CompositeScalarDeletingDestructor(void* trophy, int, unsigned int flags)
{
    CObject_004dc568* object = static_cast<CObject_004dc568*>(trophy);
    object->Member14.~C3DAnimationBlendState();
    object->Member0.~C3DAnimationBlendState();
    if (flags & 1)
        operator delete(object);
    return trophy;
}
