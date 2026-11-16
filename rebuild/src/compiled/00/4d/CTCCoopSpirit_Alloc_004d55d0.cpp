#pragma optimize("s",on)
// CTCCoopSpirit::Alloc(CThing& thing) -- the registry factory: a new CTCCoopSpirit for `thing` (operator new(0x34) 0x00BFEA1A, null-checked, then
//   the constructor 0x00670050), returned as its CTCBase
// Leaf: ?Alloc@CTCCoopSpirit@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D55D0, oracle length 29 bytes.
// Identity: Ego_r.pdb / FableWin.pdb ?Alloc@CTCCoopSpirit@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCCoopSpirit@@QAE@AAVCThing@@@Z (0x00670050, landed). (Landed earlier as CTCCoopSpirit::Alloc(CThing*) returning CTCCoopSpirit* through placement new;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCCoopSpirit : public CTCBase {
public:
    CTCCoopSpirit(CThing& thing);                        // 0x00670050
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x34 - 0xC];              // the class's own members (not modelled); sizeof(CTCCoopSpirit) = 0x34
};

CTCBase* __fastcall CTCCoopSpirit::Alloc(CThing& thing)
{
    return new CTCCoopSpirit(thing);
}
