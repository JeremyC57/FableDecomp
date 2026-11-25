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
class CInputEvent;

struct CGamePlayerInterfaceCalls {
};

struct CInterfaceOwner_Retail {
    unsigned char              _pad_0x00[0x24];
    CGamePlayerInterfaceCalls* Interface;   // +0x24
    float GetButtonPressureForEvent(CInputEvent* event);
};

// Naming (NF438 gen, 2026-11-25): callee-only rebind of this row's calls to their PDB spellings (T1 tracker; the target
//   self rows' Leaf / PDB name sets), through a bodiless declaration (no layout; rulings r2/r3):
//   0x00445800 ?GetButtonPressureForEvent@CGamePlayerInterface@@QBEMABVCInputEvent@@@Z (was ?GetButtonPressureForEvent@CGamePlayerInterfaceCalls@@QAEMPAVCInputEvent@@@Z)
class CGamePlayerInterface { public: float GetButtonPressureForEvent(CInputEvent const&) const; };
float CInterfaceOwner_Retail::GetButtonPressureForEvent(CInputEvent* event)
{
    // The named float local is what stops VC7.1 turning this into a sibling-call `jmp`.
    float pressure = reinterpret_cast<const CGamePlayerInterface*>(this->Interface)->GetButtonPressureForEvent(*(event));
    return pressure;
}