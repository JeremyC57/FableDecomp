#pragma optimize("s",on)
// CTCActionUseScriptedHook::Alloc(CThing& thing) -- the registry factory: a new CTCActionUseScriptedHook for `thing` (operator new(0x50) 0x00BFEA1A, null-checked, then
//   the constructor 0x00729A90), returned as its CTCBase
// Leaf: ?Alloc@CTCActionUseScriptedHook@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D2AD4, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x50 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCActionUseScriptedHook::CTCActionUseScriptedHook ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CTCActionUseScriptedHook@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); its only retail reference is
//   CTCBase::RegisterTCs 0x004EE23F, whose landed row already binds this spelling here (reloc_bindings.tsv). The constructor is
//   ??0CTCActionUseScriptedHook@@QAE@AAVCThing@@@Z (Ego_r + FableWin name sets), 0x00729A90, landed in the same unit.
// Naming (NF456, r454b unit with 0x00729A90): re-authored on the shared engine/CTCBase.h as the 0x004D393C pattern, same bytes (CTCBase is the
//   first base, so the upcast needs no adjustment). Was the generated free function CTCActionUseScriptedHook_Alloc(CThing*) returning CTCActionUseScriptedHook*, calling the
//   constructor as (CThing*).
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCActionUseScriptedHook : public CTCBase {
public:
    CTCActionUseScriptedHook(CThing& thing);                // 0x00729A90
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x50 - 0xC];              // the class's own members (not modelled); sizeof(CTCActionUseScriptedHook) = 0x50
};

CTCBase* __fastcall CTCActionUseScriptedHook::Alloc(CThing& thing)
{
    return new CTCActionUseScriptedHook(thing);
}
