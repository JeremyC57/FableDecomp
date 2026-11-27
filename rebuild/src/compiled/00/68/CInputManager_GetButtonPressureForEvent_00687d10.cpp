// <unattributed>::GetButtonPressureForEvent(CInputEvent*) -> Interface(+0x24)->GetButtonPressureForEvent
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00687D10, oracle length 16 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
// The manifest label for this body is propagated across byte-identical bodies and is NOT trusted;
// the class is modelled from the bytes only and the name below describes what the code does.
//
// Retail bytes:  mov eax,[esp+4] ; mov ecx,[ecx+24h] ; push eax ; call CGamePlayerInterface::GetButtonPressureForEvent ; ret 4
// Forwards the event to the player interface held at +0x24 and returns its float result (the
// callee @ 0x00445800 is a manifest-named __fastcall returning float; its label is untrusted).
// NF479 (Codex pass 29 name fixes): this is the const member ?GetButtonPressureForEvent@CInputProcessGameBase@@QBEMABVCInputEvent@@@Z (Ego_r
//   0x006B2E20 / FableWin 0x021D4160 publics; both PDBs, NAME_FIXES_QUEUE land1122p26), on the retail view of engine/CInputProcessGameBase.h
//   (PGamePlayerInterface at +0x24); was the stand-in CInterfaceOwner_Retail::GetButtonPressureForEvent(CInputEvent*). Bytes unchanged.
// Leaf: ?GetButtonPressureForEvent@CInputProcessGameBase@@QBEMABVCInputEvent@@@Z
#define FABLE_RETAIL_INPUTPROCESS
#define FABLE_RETAIL_COMBAT_INPUT_PASS26
#include "engine/CInputProcessGameBase.h"
class CGamePlayerInterface { public: float GetButtonPressureForEvent(CInputEvent const&) const; };
float CInputProcessGameBase::GetButtonPressureForEvent(const CInputEvent& event) const
{
    float pressure = PGamePlayerInterface->GetButtonPressureForEvent(event);
    return pressure;
}
