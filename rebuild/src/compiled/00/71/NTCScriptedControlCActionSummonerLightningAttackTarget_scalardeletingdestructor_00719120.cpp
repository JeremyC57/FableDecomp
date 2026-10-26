// NTCScriptedControl::CActionSummonerLightningAttackTarget::`scalar deleting destructor' @ 0x00719120 (retail, 48 bytes) -- a scalar deleting destructor body (`??_G`, flags in the
// stack slot, __thiscall shape modelled as __fastcall with a dead edx): destroys the members last-to-first,
// +0x1c CBaseClassNonCopyable (~ @ 0x0099A300), +0xc CScriptThing (~ @ 0x004AA840), then +0x0 CBaseClassNonCopyable (~ @ 0x0099A300), and frees the block with operator delete
// (MSVCR71 thunk 0x00BFE9BC) when flags & 1. Each callee is declared under its own identity (vtable RTTI /
// retail labels); the class itself is not attributed, so it is named by address.
// Engine headers for the callee classes (PDB data layouts, renamed out of the way): the local declarations
// below carry the retail identities the relocations bind.
#define CScriptThing CScriptThingPdb
#include "engine/CScriptThing.h"
#undef CScriptThing
#pragma optimize("t", on)
struct CBaseClassNonCopyable {              // vtable 0x0129A7C4 (.?AVCBaseClassNonCopyable@@); ctor 0x0099A2F0
    void* _vptr;
    ~CBaseClassNonCopyable();               // 0x0099A300 (re-points to the CBaseClass vtable)
};
class CScriptThing { public:                       // vtable 0x01238C8C (.?AVCScriptThing@@)
    void* _vptr;
    ~CScriptThing();                        // 0x004AA840
};
void __cdecl operator delete(void* block);  // MSVCR71 import thunk 0x00BFE9BC

#pragma pack(push, 1)
struct CObject_00719120 {
    CBaseClassNonCopyable Member0;
    unsigned char _padc[0x8];
    CScriptThing Memberc;                       // +0xc
    unsigned char _pad1c[0xc];
    CBaseClassNonCopyable Member1c;             // +0x1c
};
#pragma pack(pop)

void* __fastcall CActionSummonerLightningAttackTarget_CompositeScalarDeletingDestructor(void* action, int, unsigned int flags)
{
    CObject_00719120* object = static_cast<CObject_00719120*>(action);
    object->Member1c.~CBaseClassNonCopyable();
    object->Memberc.~CScriptThing();
    object->Member0.~CBaseClassNonCopyable();
    if (flags & 1)
        operator delete(object);
    return action;
}
