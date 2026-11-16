#pragma optimize("s",on)
// CTCGeneratesExperienceOnKilling::Alloc(CThing& thing) -- the registry factory: a new CTCGeneratesExperienceOnKilling for `thing` (operator new(0xc) 0x00BFEA1A, null-checked, then
//   the constructor 0x004D5769), returned as its CTCBase
// Leaf: ?Alloc@CTCGeneratesExperienceOnKilling@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D8E8C, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0xc ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCGeneratesExperienceOnKilling::CTCGeneratesExperienceOnKilling ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb / FableWin.pdb ?Alloc@CTCGeneratesExperienceOnKilling@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCGeneratesExperienceOnKilling@@QAE@AAVCThing@@@Z (0x004D5769, landed). (Landed earlier as a generated free function CTCGeneratesExperienceOnKilling_Alloc(CThing*) returning CTCGeneratesExperienceOnKilling*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCGeneratesExperienceOnKilling : public CTCBase {
public:
    CTCGeneratesExperienceOnKilling(CThing& thing);                        // 0x004D5769
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0xc - 0xC];              // the class's own members (not modelled); sizeof(CTCGeneratesExperienceOnKilling) = 0xc
};

CTCBase* __fastcall CTCGeneratesExperienceOnKilling::Alloc(CThing& thing)
{
    return new CTCGeneratesExperienceOnKilling(thing);
}
