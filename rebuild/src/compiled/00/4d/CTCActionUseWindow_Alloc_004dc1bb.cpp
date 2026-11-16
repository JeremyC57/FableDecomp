#pragma optimize("s",on)
// CTCActionUseWindow::Alloc(CThing& thing) -- the registry factory: a new CTCActionUseWindow for `thing` (operator new(0x14) 0x00BFEA1A, null-checked, then
//   the constructor 0x004DAB44), returned as its CTCBase
// Leaf: ?Alloc@CTCActionUseWindow@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DC1BB, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x14 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCActionUseWindow::CTCActionUseWindow ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb / FableWin.pdb ?Alloc@CTCActionUseWindow@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCActionUseWindow@@QAE@AAVCThing@@@Z (0x004DAB44, landed). (Landed earlier as a generated free function CTCActionUseWindow_Alloc(CThing*) returning CTCActionUseWindow*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCActionUseWindow : public CTCBase {
public:
    CTCActionUseWindow(CThing& thing);                        // 0x004DAB44
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x14 - 0xC];              // the class's own members (not modelled); sizeof(CTCActionUseWindow) = 0x14
};

CTCBase* __fastcall CTCActionUseWindow::Alloc(CThing& thing)
{
    return new CTCActionUseWindow(thing);
}
