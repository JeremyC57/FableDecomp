// CPersistable_00eb9950::PersistTransfer_HaveSpoken_00eb9950(CPersistContext* context) -- persists the "HaveSpoken" flag at +0x20
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EB9950, oracle length 33 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push ecx ; lea eax,[esp+3] ; push eax ; add ecx,0x20 ; push ecx ; mov ecx,[esp+10h] ; push offset "HaveSpoken" ; mov byte ptr [esp+0Fh],0 ; call CPersistContext::Transfer<signed char> ; pop ecx ; ret 4
// Virtual persist hook (slot 1 of this class's vtable): transfers the single byte member at
// +0x20 through CPersistContext::Transfer<signed char>(name, value, default) @ 0x004045C0,
// with a zero default built in a one-byte stack temporary. The member name is taken
// from the persisted key. One of 44 bodies of this shape.
class CPersistContext { public:
    void Transfer(const char* name, signed char& value, const signed char& defaultValue);   // Transfer<signed char> @ 0x004045C0
};

struct CPersistable_00eb9950 {
    unsigned char _pad[0x1c];        // after the vptr; members not touched here
    signed char m_bHaveSpoken;                // +0x20
    virtual void v00();
    virtual void PersistTransfer_HaveSpoken_00eb9950(CPersistContext* context);
};

void CPersistable_00eb9950::PersistTransfer_HaveSpoken_00eb9950(CPersistContext* context)
{
    signed char defaultValue = 0;
    context->Transfer("HaveSpoken", m_bHaveSpoken, defaultValue);
}