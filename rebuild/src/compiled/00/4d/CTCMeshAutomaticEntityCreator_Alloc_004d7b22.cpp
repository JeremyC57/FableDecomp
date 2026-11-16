#pragma optimize("s",on)
// CTCMeshAutomaticEntityCreator::Alloc(CThing& thing) -- the registry factory: a new CTCMeshAutomaticEntityCreator for `thing` (operator new(0xc) 0x00BFEA1A, null-checked, then
//   the constructor 0x004D3161), returned as its CTCBase
// Leaf: ?Alloc@CTCMeshAutomaticEntityCreator@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D7B22, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0xc ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCMeshAutomaticEntityCreator::CTCMeshAutomaticEntityCreator ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb / FableWin.pdb ?Alloc@CTCMeshAutomaticEntityCreator@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCMeshAutomaticEntityCreator@@QAE@AAVCThing@@@Z (0x004D3161, landed). (Landed earlier as a generated free function CTCMeshAutomaticEntityCreator_Alloc(CThing*) returning CTCMeshAutomaticEntityCreator*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCMeshAutomaticEntityCreator : public CTCBase {
public:
    CTCMeshAutomaticEntityCreator(CThing& thing);                        // 0x004D3161
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0xc - 0xC];              // the class's own members (not modelled); sizeof(CTCMeshAutomaticEntityCreator) = 0xc
};

CTCBase* __fastcall CTCMeshAutomaticEntityCreator::Alloc(CThing& thing)
{
    return new CTCMeshAutomaticEntityCreator(thing);
}
