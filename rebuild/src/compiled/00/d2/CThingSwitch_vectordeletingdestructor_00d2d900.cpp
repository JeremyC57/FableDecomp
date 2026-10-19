// CThingSwitch::`vector_deleting_destructor' @ 0x00D2D900 (retail, 48 bytes) -- a scalar deleting destructor body (`??_G`, flags in the
// stack slot, __thiscall shape modelled as __fastcall with a dead edx): destroys the members last-to-first,
// +0x28 CVector_008ac970 (~ @ 0x008AC970), +0x1c CVector_008ac970 (~ @ 0x008AC970), then the base NUISystem::CFrontEndScreen (~ @ 0x00F35B40), and frees the block with operator delete
// (MSVCR71 thunk 0x00BFE9BC) when flags & 1. Each callee is declared under its own identity (vtable RTTI /
// retail labels); the class itself is not attributed, so it is named by address.
// Engine headers for the callee classes (PDB data layouts, renamed out of the way): the local declarations
// below carry the retail identities the relocations bind.
#define CFrontEndScreen CFrontEndScreenPdb
#include "engine/CFrontEndScreen.h"
#undef CFrontEndScreen
#pragma optimize("t", on)
namespace NUISystem {
struct CFrontEndScreen {                   // vtable 0x012C3224; label NUISystem::CFrontEndScreen::~CFrontEndScreen
    void* _vptr;
    ~CFrontEndScreen();                     // 0x00F35B40 (inlines the CScriptThing member destructor at +8)
};
}
struct CVector_008ac970 {                   // unattributed vector of polymorphic elements (slot-0 destroy with flag 0, as vector<CConversation>)
    void* _M_start; void* _M_finish; void* _M_end_of_storage;
    ~CVector_008ac970();                    // 0x008AC970 (class not identified)
};
void __cdecl operator delete(void* block);  // MSVCR71 import thunk 0x00BFE9BC

#pragma pack(push, 1)
struct CObject_00d2d900 : NUISystem::CFrontEndScreen {
    unsigned char _pad1c[0x18];
    CVector_008ac970 Member1c;                  // +0x1c
    CVector_008ac970 Member28;                  // +0x28
};
#pragma pack(pop)

void* __fastcall CThingSwitch_CompositeScalarDeletingDestructor(void* self, int, unsigned int flags)
{
    CObject_00d2d900* object = static_cast<CObject_00d2d900*>(self);
    object->Member28.~CVector_008ac970();
    object->Member1c.~CVector_008ac970();
    object->NUISystem::CFrontEndScreen::~CFrontEndScreen();
    if (flags & 1)
        operator delete(object);
    return self;
}
