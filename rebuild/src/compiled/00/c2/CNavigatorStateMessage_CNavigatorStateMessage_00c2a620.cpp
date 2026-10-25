// CNavigatorStateMessage::CNavigatorStateMessage(unsigned char state) -- the 8-byte navigator state message: its vtable (0x012B4178) and the state byte at +4
// Leaf: ??0CNavigatorStateMessage@@QAE@E@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C2A620, 18 bytes, __thiscall, `ret 4`: mov eax,ecx ; mov cl,[esp+4] ;
//   mov dword ptr [eax],12B4178h ; mov [eax+4],cl ; ret 4
// Naming (sweep1, 2026-10-25): was landed as CTCPhysicsNavigator::EnableNavigator (a propagated manifest label; the
//   stand-in stored an extern word as the vtable and returned `this`); descriptive (no PDB or RTTI covers libpk).
//   FINDINGS chunk 436: its only caller is the landed CNavigatorSwitch::SendState 0x00C2A7A0 (0x00C2A7C3), which builds
//   this message on its stack from the switch's state byte and passes it to the receiver's slot 10; SendState carries
//   this exact constructor as its same-TU body. A constructor returns `this` in eax and the compiler emits the vtable.
struct CNavigatorStateMessage {                 // vtable 0x012B4178, 8 bytes
    virtual void v00();
    CNavigatorStateMessage(unsigned char state);
    unsigned char m_state;                      // +0x04
};

CNavigatorStateMessage::CNavigatorStateMessage(unsigned char state)
    : m_state(state)
{
}
