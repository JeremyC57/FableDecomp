// CThingSwitch::`vector_deleting_destructor' @ 0x00E5DE50 (retail, 48 bytes) -- a scalar deleting destructor body (`??_G`, flags in the
// Leaf: `vector_deleting_destructor'
// stack slot, __thiscall shape modelled as __fastcall with a dead edx): destroys the members last-to-first,
// +0x34 CTimer (~ @ 0x00CD4470), +0x30 CTimer (~ @ 0x00CD4470), then the base NUISystem::CFrontEndScreen (~ @ 0x00F35B40), and frees the block with operator delete
// (MSVCR71 thunk 0x00BFE9BC) when flags & 1. Each callee is declared under its own identity (vtable RTTI /
// retail labels); the class itself is not attributed, so it is named by address.
// Engine headers for the callee classes (PDB data layouts, renamed out of the way): the local declarations
// below carry the retail identities the relocations bind.
#define CFrontEndScreen CFrontEndScreenPdb
#include "engine/CFrontEndScreen.h"
#undef CFrontEndScreen
#pragma optimize("t", on)
namespace NUISystem {
class CFrontEndScreen { public:                   // vtable 0x012C3224; label NUISystem::CFrontEndScreen::~CFrontEndScreen
    void* _vptr;
    ~CFrontEndScreen();                     // 0x00F35B40 (inlines the CScriptThing member destructor at +8)
};
}
class CTimer { public:                             // one word: the timer handle
    long Handle;
    ~CTimer();                              // 0x00CD4470 (hands the handle back through the global at 0x0143E8F8, slot 0x160)
};
void __cdecl operator delete(void* block);  // MSVCR71 import thunk 0x00BFE9BC

#pragma pack(push, 1)
struct CObject_00e5de50 : NUISystem::CFrontEndScreen {
    unsigned char _pad30[0x2c];
    CTimer Member30;                            // +0x30
    CTimer Member34;                            // +0x34
};
#pragma pack(pop)

void* __fastcall CThingSwitch_CompositeScalarDeletingDestructor(void* self, int, unsigned int flags)
{
    CObject_00e5de50* object = static_cast<CObject_00e5de50*>(self);
    object->Member34.~CTimer();
    object->Member30.~CTimer();
    object->NUISystem::CFrontEndScreen::~CFrontEndScreen();
    if (flags & 1)
        operator delete(object);
    return self;
}
