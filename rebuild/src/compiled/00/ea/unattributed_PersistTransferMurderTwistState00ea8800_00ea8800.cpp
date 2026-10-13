// CPersistable_00ea8800::PersistTransfer_MurderTwistState_00ea8800(CPersistContext* context) -- persists the MurderTwistState dword at +0x4c with a zero default
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EA8800, oracle length 35 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push ecx ; lea eax,[esp] ; push eax ; add ecx,0x4c ; push ecx ; mov ecx,[esp+10h] ; push offset "MurderTwistState" ; mov dword ptr [esp+0Ch],0 ; call CPersistContext::Transfer<long> ; pop ecx ; ret 4
// Virtual persist hook (slot 1 of this class's vtable): transfers the dword member at +0x4c
// through CPersistContext::Transfer<long>(name, value, default) @ 0x00410BE0, with a zero
// default built in a dword stack temporary. The member name is taken from the
// persisted key; the member type is any 4-byte scalar (`long` models it). One of 16 bodies.
struct CPersistContext {
    void Transfer(const char* name, long& value, const long& defaultValue);   // Transfer<long> @ 0x00410BE0
};

struct CPersistable_00ea8800 {
    unsigned char _pad[0x48];        // after the vptr; members not touched here
    long m_MurderTwistState;                        // +0x4c
    virtual void v00();
    virtual void PersistTransfer_MurderTwistState_00ea8800(CPersistContext* context);
};

void CPersistable_00ea8800::PersistTransfer_MurderTwistState_00ea8800(CPersistContext* context)
{
    long defaultValue = 0;
    context->Transfer("MurderTwistState", m_MurderTwistState, defaultValue);
}