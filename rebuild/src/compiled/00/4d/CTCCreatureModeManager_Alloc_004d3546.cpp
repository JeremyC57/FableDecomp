#pragma optimize("s",on)
// CTCCreatureModeManager::Alloc(CThing& thing) -- the registry factory: a new CTCCreatureModeManager for `thing` (operator new(0x24) 0x00BFEA1A, null-checked, then
//   the constructor 0x0073AF00), returned as its CTCBase
// Leaf: ?Alloc@CTCCreatureModeManager@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D3546, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x24 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCCreatureModeManager::CTCCreatureModeManager ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CTCCreatureModeManager@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCCreatureModeManager@@QAE@AAVCThing@@@Z (0x0073AF00, landed). (Landed earlier as a generated free function CTCCreatureModeManager_Alloc(CThing*) returning CTCCreatureModeManager*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCCreatureModeManager : public CTCBase {
public:
    CTCCreatureModeManager(CThing& thing);                        // 0x0073AF00
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x24 - 0xC];              // the class's own members (not modelled); sizeof(CTCCreatureModeManager) = 0x24
};

CTCBase* __fastcall CTCCreatureModeManager::Alloc(CThing& thing)
{
    return new CTCCreatureModeManager(thing);
}
