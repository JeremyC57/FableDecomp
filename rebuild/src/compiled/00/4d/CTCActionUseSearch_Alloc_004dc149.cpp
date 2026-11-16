#pragma optimize("s",on)
// CTCActionUseSearch::Alloc(CThing& thing) -- the registry factory: a new CTCActionUseSearch for `thing` (operator new(0x14) 0x00BFEA1A, null-checked, then
//   the constructor 0x004DAAF4), returned as its CTCBase
// Leaf: ?Alloc@CTCActionUseSearch@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DC149, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x14 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCActionUseSearch::CTCActionUseSearch ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CTCActionUseSearch@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCActionUseSearch@@QAE@AAVCThing@@@Z (0x004DAAF4, landed). (Landed earlier as a generated free function CTCActionUseSearch_Alloc(CThing*) returning CTCActionUseSearch*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCActionUseSearch : public CTCBase {
public:
    CTCActionUseSearch(CThing& thing);                        // 0x004DAAF4
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x14 - 0xC];              // the class's own members (not modelled); sizeof(CTCActionUseSearch) = 0x14
};

CTCBase* __fastcall CTCActionUseSearch::Alloc(CThing& thing)
{
    return new CTCActionUseSearch(thing);
}
