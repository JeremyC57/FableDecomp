// CPersistable_00e4b400::PersistTransfer_TollState_00e4b400(CPersistContext* context) -- persists the TollState dword at +0x1c with a zero default
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E4B400, oracle length 35 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push ecx ; lea eax,[esp] ; push eax ; add ecx,0x1c ; push ecx ; mov ecx,[esp+10h] ; push offset "TollState" ; mov dword ptr [esp+0Ch],0 ; call CPersistContext::Transfer<long> ; pop ecx ; ret 4
// Virtual persist hook (slot 1 of this class's vtable): transfers the dword member at +0x1c
// through CPersistContext::Transfer<long>(name, value, default) @ 0x00410BE0, with a zero
// default built in a dword stack temporary. The member name is taken from the
// persisted key; the member type is any 4-byte scalar (`long` models it). One of 16 bodies.
class CPersistContext { public:
    template <class T> void Transfer(const char* name, T& value, const T& defaultValue);   // 0x00410BE0 <long>
};

struct CPersistable_00e4b400 {
    unsigned char _pad[0x18];        // after the vptr; members not touched here
    long m_TollState;                        // +0x1c
    virtual void v00();
    virtual void PersistTransfer_TollState_00e4b400(CPersistContext* context);
};

void CPersistable_00e4b400::PersistTransfer_TollState_00e4b400(CPersistContext* context)
{
    long defaultValue = 0;
    context->Transfer("TollState", m_TollState, defaultValue);
}