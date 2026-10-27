// CDispatcherB_00c2c7e0::Register(CEvent_00c2c7e0*) -- wrap the argument in a CVtable_012b44b8 message and pass its address to the receiver embedded at +0x4 (virtual slot 10)
// Leaf: ?Register@CDispatcherB_00c2c7e0@@QAEXPAUCEvent_00c2c7e0@@@Z
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C2BCF0, 77 bytes, thiscall, `ret 4`, EH state 0 while the message lives.
// Identity (descriptive): the name reloc_bindings already gives 0x00C2BCF0 (the landed caller CCombatEventSource_00c2c7e0::CreateParryBlockEvent 0x00C2C7E0 registers each new event with the world's +0xC dispatcher through it). libpk has no RTTI and no PDB; the message type keeps the name its landed constructor
//   0x00C2BC60 carries (vtable 0x012B44B8).
// Callees: CVtable_012b44b8::CVtable_012b44b8(void*) 0x00C2BC60; the receiver's virtual slot 10 (+0x28) taking the message by address.
// Shape (family K061, representative 0x00C2BCF0): the landed CNavigatorSwitch::SendState 0x00C2A7A0 -- the message
//   constructor's body is compiled out of line earlier in this TU, so VC7.1 knows it leaves edx alone and parks `this`
//   there (`mov edx,ecx ; ... call ; lea ecx,[edx+4h]`); the message's empty inline destructor emits no call.
struct CVtable_012b44b8 {                                         // vtable 0x012B44B8, 8 bytes
    virtual void v00();
    CVtable_012b44b8(void* first);                               // 0x00C2BC60
    ~CVtable_012b44b8() { }
    void* m_first;                                       // +0x04
};
struct CUnprovenPKReceiver_00c2bcf0 {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void Receive(CVtable_012b44b8* message);             // slot 10 (+0x28)
};
struct CEvent_00c2c7e0;
struct CDispatcherB_00c2c7e0 {
    unsigned char _pad[0x4];
    CUnprovenPKReceiver_00c2bcf0 m_receiver;               // +0x4 (embedded)
    void Register(CEvent_00c2c7e0* argument);
};
// Same-TU body of the message constructor (retail 0x00C2BC60, landed separately): kept out of line so that the caller
// sees its register use. Only Register is graded from this file.
#pragma auto_inline(off)
CVtable_012b44b8::CVtable_012b44b8(void* first)
    : m_first(first)
{
}
#pragma auto_inline(on)
void CDispatcherB_00c2c7e0::Register(CEvent_00c2c7e0* argument)
{
    CVtable_012b44b8 message(argument);
    m_receiver.Receive(&message);
}