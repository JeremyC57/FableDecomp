#pragma optimize("s",on)
// CTCDPhysicalObstruction::Alloc(CThing& thing) -- the registry factory: a new CTCDPhysicalObstruction for `thing` (operator new(0x50) 0x00BFEA1A, null-checked, then
//   the constructor 0x004DF14B), returned as its CTCBase
// Leaf: ?Alloc@CTCDPhysicalObstruction@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E15D3, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x50 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCDPhysicalObstruction::CTCDPhysicalObstruction ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb / FableWin.pdb ?Alloc@CTCDPhysicalObstruction@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCDPhysicalObstruction@@QAE@AAVCThing@@@Z (0x004DF14B, landed). (Landed earlier as a generated free function CTCDPhysicalObstruction_Alloc(CThing*) returning CTCDPhysicalObstruction*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCDPhysicalObstruction : public CTCBase {
public:
    CTCDPhysicalObstruction(CThing& thing);                        // 0x004DF14B
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x50 - 0xC];              // the class's own members (not modelled); sizeof(CTCDPhysicalObstruction) = 0x50
};

CTCBase* __fastcall CTCDPhysicalObstruction::Alloc(CThing& thing)
{
    return new CTCDPhysicalObstruction(thing);
}
