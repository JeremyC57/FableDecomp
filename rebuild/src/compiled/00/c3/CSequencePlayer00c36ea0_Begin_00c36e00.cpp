// CSequencePlayer_00c36ea0::Begin(CSequenceClock*) -- wrap the argument in a CVtable_012b51d0 message and pass its address to the receiver embedded at +0x10 (virtual slot 10)
// Leaf: ?Begin@CSequencePlayer_00c36ea0@@QAEXPAUCSequenceClock@@@Z
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C36E00, 77 bytes, thiscall, `ret 4`, EH state 0 while the message lives.
// Identity (descriptive): the name reloc_bindings already gives 0x00C36E00 (the landed caller CSequencePlayer_00c36ea0::ExecuteAttackSequence 0x00C36EA0 calls Begin(&Clock)). libpk has no RTTI and no PDB; the message type keeps the name its landed constructor
//   0x00C36D60 carries (vtable 0x012B51D0).
// Callees: CVtable_012b51d0::CVtable_012b51d0(void*) 0x00C36D60; the receiver's virtual slot 10 (+0x28) taking the message by address.
// Shape (family K061, representative 0x00C2BCF0): the landed CNavigatorSwitch::SendState 0x00C2A7A0 -- the message
//   constructor's body is compiled out of line earlier in this TU, so VC7.1 knows it leaves edx alone and parks `this`
//   there (`mov edx,ecx ; ... call ; lea ecx,[edx+10h]`); the message's empty inline destructor emits no call.
struct CVtable_012b51d0 {                                         // vtable 0x012B51D0, 8 bytes
    virtual void v00();
    CVtable_012b51d0(void* first);                               // 0x00C36D60
    ~CVtable_012b51d0() { }
    void* m_first;                                       // +0x04
};
struct CUnprovenPKReceiver_00c36e00 {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void Receive(CVtable_012b51d0* message);             // slot 10 (+0x28)
};
struct CSequenceClock;
struct CSequencePlayer_00c36ea0 {
    unsigned char _pad[0x10];
    CUnprovenPKReceiver_00c36e00 m_receiver;               // +0x10 (embedded)
    void Begin(CSequenceClock* argument);
};
// Same-TU body of the message constructor (retail 0x00C36D60, landed separately): kept out of line so that the caller
// sees its register use. Only Begin is graded from this file.
#pragma auto_inline(off)
CVtable_012b51d0::CVtable_012b51d0(void* first)
    : m_first(first)
{
}
#pragma auto_inline(on)
void CSequencePlayer_00c36ea0::Begin(CSequenceClock* argument)
{
    CVtable_012b51d0 message(argument);
    m_receiver.Receive(&message);
}