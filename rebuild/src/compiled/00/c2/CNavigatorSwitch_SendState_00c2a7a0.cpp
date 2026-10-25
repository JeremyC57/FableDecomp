// CNavigatorSwitch::SendState(unsigned char unused) -- build an 8-byte state message (vtable 0x012B4178) from the switch's state byte (+0x04) and hand it to the receiver embedded at +0x08 (virtual slot 10); the argument is not read
// Leaf: SendState
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C2A7A0, length 80 bytes (77 + int3 padding), __fastcall, `ret 4`.
// Caller: 0x00C2A890 (`OnActivate @ 0x00C2A700 ; SendState(m_state) ; m_state = 0 ; jmp OnActivate2 @ 0x00C2A750`: it pushes the
//   zero-extended state byte, which this body ignores and re-reads from +0x04).
// Callees: the message constructor @ 0x00C2A620 (landed as "CTCPhysicsNavigator::EnableNavigator": vtable store + one byte,
//   `mov cl,[esp+4]`, returns this), the receiver's virtual slot 10 (+0x28) taking the message by address.
// Name: no PDB names cover this object; the names are descriptive (the catalog's `LoadSea_EnableNavigator` is propagated).
// Shape: /EHsc -- EH state 0 while the message lives; its destructor is empty and inline, so no call follows. `this` is
//   parked in edx across the constructor call because the constructor's body is in this TU (compiled first, out of line)
//   and is known to leave edx alone (`mov edx,ecx ; ... call ; lea ecx,[edx+8]`); an `unsigned char` argument is pushed
//   as `xor eax,eax ; mov al,[edx+4] ; push eax` (an int parameter gives `movzx eax,byte ptr [edx+4]` and schedules the
//   `lea ecx` after the push). The caller 0x00C2A890 pushes its argument the same way, so it is an `unsigned char` too.
struct CNavigatorStateMessage {             // vtable 0x012B4178, 8 bytes
    virtual void v00();
    CNavigatorStateMessage(unsigned char state);   // 0x00C2A620
    ~CNavigatorStateMessage() { }
    unsigned char m_state;                  // +0x04
};

struct CNavigatorReceiver {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void Receive(CNavigatorStateMessage* message);   // slot 10 (+0x28)
};

struct CNavigatorSwitch {
    void*              __vftable;           // +0x00
    unsigned char      m_state;             // +0x04
    unsigned char      _pad_0x05[3];
    CNavigatorReceiver m_receiver;          // +0x08 (embedded)
    void SendState(unsigned char unused);
};

// Same-TU body of the message constructor (retail 0x00C2A620, landed separately): kept out of line so that the caller
// sees its register use. Only SendState is graded from this file.
#pragma auto_inline(off)
CNavigatorStateMessage::CNavigatorStateMessage(unsigned char state)
    : m_state(state)
{
}
#pragma auto_inline(on)

void CNavigatorSwitch::SendState(unsigned char)
{
    CNavigatorStateMessage message(m_state);
    m_receiver.Receive(&message);
}