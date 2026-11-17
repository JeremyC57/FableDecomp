// CThingSwitch::`vector_deleting_destructor' @ 0x00D2D900 (retail, 48 bytes) -- a scalar deleting destructor body (`??_G`, flags in the
// Leaf: ?CThingSwitch_CompositeScalarDeletingDestructor@@YIPAXPAXHI@Z
// stack slot, __thiscall shape modelled as __fastcall with a dead edx): destroys the members last-to-first,
// +0x28 vector<CScriptThing> (~ @ 0x008AC970), +0x1c vector<CScriptThing> (~ @ 0x008AC970), then the base NUISystem::CFrontEndScreen (~ @ 0x00F35B40), and frees the block with operator delete
// (MSVCR71 thunk 0x00BFE9BC) when flags & 1. Each callee is declared under its own identity (vtable RTTI /
// retail labels); the class itself is not attributed, so it is named by address.
// Engine headers for the callee classes (PDB data layouts, renamed out of the way): the local declarations
// below carry the retail identities the relocations bind.
// NF48 (round 7): the two member vectors are STLport 4.6.2 std::vector<CScriptThing> (shared stlport/fable_containers.h, included
// first so the overlay opt-in applies; engine/CScriptThing.h for the element; a local CVector_008ac970 stand-in before); their
// destructor is ~vector<CScriptThing> 0x008AC970 (landed), called out of line (FABLE_RETAIL_STLP_VECTOR_DTOR_OUTOFLINE).
#define FABLE_RETAIL_STLP_VECTOR_DTOR_OUTOFLINE
#include "stlport/fable_containers.h"
class CCPPointerInfo;
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
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
void __cdecl operator delete(void* block);  // MSVCR71 import thunk 0x00BFE9BC

#pragma pack(push, 1)
struct CObject_00d2d900 : NUISystem::CFrontEndScreen {
    unsigned char _pad1c[0x18];
    std::vector<CScriptThing> Member1c;                  // +0x1c
    std::vector<CScriptThing> Member28;                  // +0x28
};
#pragma pack(pop)

void* __fastcall CThingSwitch_CompositeScalarDeletingDestructor(void* self, int, unsigned int flags)
{
    CObject_00d2d900* object = static_cast<CObject_00d2d900*>(self);
    object->Member28.std::vector<CScriptThing>::~vector();
    object->Member1c.std::vector<CScriptThing>::~vector();
    object->NUISystem::CFrontEndScreen::~CFrontEndScreen();
    if (flags & 1)
        operator delete(object);
    return self;
}
