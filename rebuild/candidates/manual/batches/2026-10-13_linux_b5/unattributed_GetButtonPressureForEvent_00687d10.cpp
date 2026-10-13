// <unattributed>::GetButtonPressureForEvent(CInputEvent*) -> Interface(+0x24)->GetButtonPressureForEvent
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00687D10, oracle length 16 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
// The manifest label for this body is propagated across byte-identical bodies and is NOT trusted;
// the class is modelled from the bytes only and the name below describes what the code does.
//
// Retail bytes:  mov eax,[esp+4] ; mov ecx,[ecx+24h] ; push eax ; call CGamePlayerInterface::GetButtonPressureForEvent ; ret 4
// Forwards the event to the player interface held at +0x24 and returns its float result (the
// callee @ 0x00445800 is a manifest-named __thiscall returning float; its label is untrusted).
struct CInputEvent;

struct CGamePlayerInterfaceCalls {
    float GetButtonPressureForEvent(CInputEvent* event);   // 0x00445800
};

struct CInterfaceOwner_Retail {
    unsigned char              _pad_0x00[0x24];
    CGamePlayerInterfaceCalls* Interface;   // +0x24
    float GetButtonPressureForEvent(CInputEvent* event);
};

float CInterfaceOwner_Retail::GetButtonPressureForEvent(CInputEvent* event)
{
    // The named float local is what stops VC7.1 turning this into a sibling-call `jmp`.
    float pressure = this->Interface->GetButtonPressureForEvent(event);
    return pressure;
}
