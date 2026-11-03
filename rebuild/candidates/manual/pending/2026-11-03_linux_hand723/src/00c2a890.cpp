// CNavigatorSwitch::SendStateAndReset() -- send the 0x012B4168 message to the second receiver, send the current state byte, clear it, then send the 0x012B4170 message to the second receiver
// Leaf: ?SendStateAndReset@CNavigatorSwitch@@QAEXXZ
// Flags: none
// Grade: RELOCATION_MATCH (q.py, cl 13.10.3077, no flags)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C2A890, length 33 bytes, __thiscall, tail jump into the last sender.
// Callees (all members of the same libpk object, descriptive names -- no PDB or RTTI covers libpk): 0x00C2A700 builds a 4-byte message
//   (vtable 0x012B4168) and hands it to the receiver at +0x38 (virtual slot 10); CNavigatorSwitch::SendState(unsigned char) 0x00C2A7A0
//   (landed, ?SendState@CNavigatorSwitch@@QAEXE@Z; it ignores its argument and re-reads +0x04); 0x00C2A750 = 0x00C2A700 with the
//   message vtable 0x012B4170.
// Name: descriptive. The catalog row "CEngineWaterRenderer::LoadSea" is a propagated label: this is libpk code (the 0x00C2xxxx
//   navigator/agent block next to the landed CNavigatorSwitch::SendState, whose own header comment already names this caller).
// Shape: the state byte is pushed as `xor eax,eax ; mov al,[esi+4]` (an unsigned char argument, as SendState's comment proves).
struct CNavigatorSwitch {
    void*         __vftable;                 // +0x00
    unsigned char m_state;                   // +0x04 (set to 1 by the constructor 0x00C2A8C0)
    void SendMessage012b4168();              // 0x00C2A700
    void SendMessage012b4170();              // 0x00C2A750
    void SendState(unsigned char unused);    // 0x00C2A7A0
    void SendStateAndReset();
};

void CNavigatorSwitch::SendStateAndReset()
{
    SendMessage012b4168();
    SendState(m_state);
    m_state = 0;
    SendMessage012b4170();
}
