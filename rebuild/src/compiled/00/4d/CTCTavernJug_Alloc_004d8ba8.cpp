#pragma optimize("s",on)
// CTCTavernJug::Alloc(CThing& thing) -- the registry factory: a new CTCTavernJug for `thing` (operator new(0x38) 0x00BFEA1A, null-checked, then
//   the constructor 0x004D51C8), returned as its CTCBase
// Leaf: ?Alloc@CTCTavernJug@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D8BA8, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x38 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCTavernJug::CTCTavernJug ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb / FableWin.pdb ?Alloc@CTCTavernJug@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCTavernJug@@QAE@AAVCThing@@@Z (0x004D51C8, landed). (Landed earlier as a generated free function CTCTavernJug_Alloc(CThing*) returning CTCTavernJug*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCTavernJug : public CTCBase {
public:
    CTCTavernJug(CThing& thing);                        // 0x004D51C8
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x38 - 0xC];              // the class's own members (not modelled); sizeof(CTCTavernJug) = 0x38
};

CTCBase* __fastcall CTCTavernJug::Alloc(CThing& thing)
{
    return new CTCTavernJug(thing);
}
