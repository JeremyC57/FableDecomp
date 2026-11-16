#pragma optimize("s",on)
// CTCLightningNode::Alloc(CThing& thing) -- the registry factory: a new CTCLightningNode for `thing` (operator new(0x38) 0x00BFEA1A, null-checked, then
//   the constructor 0x004E6D17), returned as its CTCBase
// Leaf: ?Alloc@CTCLightningNode@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E7666, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x38 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCLightningNode::CTCLightningNode ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CTCLightningNode@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCLightningNode@@QAE@AAVCThing@@@Z (0x004E6D17, landed). (Landed earlier as a generated free function CTCLightningNode_Alloc(CThing*) returning CTCLightningNode*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCLightningNode : public CTCBase {
public:
    CTCLightningNode(CThing& thing);                        // 0x004E6D17
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x38 - 0xC];              // the class's own members (not modelled); sizeof(CTCLightningNode) = 0x38
};

CTCBase* __fastcall CTCLightningNode::Alloc(CThing& thing)
{
    return new CTCLightningNode(thing);
}
