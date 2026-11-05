// CThingSwitch::Alloc() -- class-factory hook: CThing's pooled operator new(sizeof(CThingSwitch) = 0xE8, false), then the out-of-line default constructor
// Leaf: ?Alloc@CThingSwitch@@SIPAVCClass@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0052ACE0, 26 bytes, plain `ret` (`static CClass* __fastcall`).
// Identity: CThingManager::InitThingTypes 0x00522A20 stores this address with the "SWITCH" type name (0x00522F44:
//   `mov [esp+1Ch],0x52ACE0` after the CCharString("SWITCH") temporary), and its constructor is the landed
//   CThingSwitch::CThingSwitch() 0x00529B00 (vtable 0x012453FC). Ego_r.pdb / FableWin.pdb ?Alloc@CThingSwitch@@SIPAVCClass@@XZ.
// Callees: CThing::operator new(unsigned int, bool) @ 0x004C7380 (`??2CThing@@KIPAXI_N@Z`, protected static __fastcall:
//   size in ecx, the bool in dl), CThingSwitch::CThingSwitch() @ 0x00529B00.
// Shape: the landed CThingObject::Alloc 0x00527320: `new (false) CThingSwitch` keeps its null test and tail-jumps to the
//   constructor (which returns `this`).
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_OPERATORNEW
#include "engine/CThing.h"

class CThingSwitch : public CThing {
public:
    unsigned char _body2[0xE8 - 0x98];                                  // sizeof(CThingSwitch) = 0xE8 in retail
    CThingSwitch();                                                      // 0x00529B00
    static CClass* __fastcall Alloc();
};

CClass* __fastcall CThingSwitch::Alloc()
{
    return new (false) CThingSwitch;
}