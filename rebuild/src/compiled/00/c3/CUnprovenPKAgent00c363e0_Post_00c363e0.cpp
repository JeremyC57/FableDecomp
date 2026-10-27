// CUnprovenPKAgent_00c363e0::Post(void*) -- wrap the argument in a CVtable_012b50e4 message and pass its address to the receiver embedded at +0x14 (virtual slot 10)
// Leaf: ?Post@CUnprovenPKAgent_00c363e0@@QAEXPAX@Z
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C363E0, 77 bytes, thiscall, `ret 4`, EH state 0 while the message lives.
// Identity (descriptive): no binding; its only reference is the tail jump from 0x00C36490 (manifest SteeringAgentList_Constructor; landed as CSequenceTrack::Finish). libpk has no RTTI and no PDB; the message type keeps the name its landed constructor
//   0x00C362D0 carries (vtable 0x012B50E4).
// Callees: CVtable_012b50e4::CVtable_012b50e4(void*) 0x00C362D0; the receiver's virtual slot 10 (+0x28) taking the message by address.
// Shape (family K061, representative 0x00C2BCF0): the landed CNavigatorSwitch::SendState 0x00C2A7A0 -- the message
//   constructor's body is compiled out of line earlier in this TU, so VC7.1 knows it leaves edx alone and parks `this`
//   there (`mov edx,ecx ; ... call ; lea ecx,[edx+14h]`); the message's empty inline destructor emits no call.
struct CVtable_012b50e4 {                                         // vtable 0x012B50E4, 8 bytes
    virtual void v00();
    CVtable_012b50e4(void* first);                               // 0x00C362D0
    ~CVtable_012b50e4() { }
    void* m_first;                                       // +0x04
};
struct CUnprovenPKReceiver_00c363e0 {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void Receive(CVtable_012b50e4* message);             // slot 10 (+0x28)
};
struct CUnprovenPKAgent_00c363e0 {
    unsigned char _pad[0x14];
    CUnprovenPKReceiver_00c363e0 m_receiver;               // +0x14 (embedded)
    void Post(void* argument);
};
// Same-TU body of the message constructor (retail 0x00C362D0, landed separately): kept out of line so that the caller
// sees its register use. Only Post is graded from this file.
#pragma auto_inline(off)
CVtable_012b50e4::CVtable_012b50e4(void* first)
    : m_first(first)
{
}
#pragma auto_inline(on)
void CUnprovenPKAgent_00c363e0::Post(void* argument)
{
    CVtable_012b50e4 message(argument);
    m_receiver.Receive(&message);
}