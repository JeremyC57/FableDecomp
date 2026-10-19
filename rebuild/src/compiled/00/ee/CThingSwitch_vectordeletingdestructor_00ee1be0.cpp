// CThingSwitch::`vector_deleting_destructor' @ 0x00EE1BE0 (retail, 48 bytes) -- a scalar deleting destructor body (`??_G`, flags in the
// stack slot, __thiscall shape modelled as __fastcall with a dead edx): destroys the members last-to-first,
// +0x28 CScriptThing (~ @ 0x004AA840), +0x1c CScriptThing (~ @ 0x004AA840), then the base NUISystem::CFrontEndScreen (~ @ 0x00F35B40), and frees the block with operator delete
// (MSVCR71 thunk 0x00BFE9BC) when flags & 1. Each callee is declared under its own identity (vtable RTTI /
// retail labels); the class itself is not attributed, so it is named by address.
// Engine headers for the callee classes (PDB data layouts, renamed out of the way): the local declarations
// below carry the retail identities the relocations bind.
#define CFrontEndScreen CFrontEndScreenPdb
#include "engine/CFrontEndScreen.h"
#undef CFrontEndScreen
#define CScriptThing CScriptThingPdb
#include "engine/CScriptThing.h"
#undef CScriptThing
#pragma optimize("t", on)
namespace NUISystem {
struct CFrontEndScreen {                   // vtable 0x012C3224; label NUISystem::CFrontEndScreen::~CFrontEndScreen
    void* _vptr;
    ~CFrontEndScreen();                     // 0x00F35B40 (inlines the CScriptThing member destructor at +8)
};
}
struct CScriptThing {                       // vtable 0x01238C8C (.?AVCScriptThing@@)
    void* _vptr;
    ~CScriptThing();                        // 0x004AA840
};
void __cdecl operator delete(void* block);  // MSVCR71 import thunk 0x00BFE9BC

#pragma pack(push, 1)
struct CObject_00ee1be0 : NUISystem::CFrontEndScreen {
    unsigned char _pad1c[0x18];
    CScriptThing Member1c;                      // +0x1c
    unsigned char _pad28[0x8];
    CScriptThing Member28;                      // +0x28
};
#pragma pack(pop)

void* __fastcall CThingSwitch_CompositeScalarDeletingDestructor(void* self, int, unsigned int flags)
{
    CObject_00ee1be0* object = static_cast<CObject_00ee1be0*>(self);
    object->Member28.~CScriptThing();
    object->Member1c.~CScriptThing();
    object->NUISystem::CFrontEndScreen::~CFrontEndScreen();
    if (flags & 1)
        operator delete(object);
    return self;
}
