// CPersistable_00eb69d0::PersistTransfer_HasBeenSpokenTo_00eb69d0(CPersistContext* context) -- persists the "HasBeenSpokenTo" flag at +0x30
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EB69D0, oracle length 33 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push ecx ; lea eax,[esp+3] ; push eax ; add ecx,0x30 ; push ecx ; mov ecx,[esp+10h] ; push offset "HasBeenSpokenTo" ; mov byte ptr [esp+0Fh],0 ; call CPersistContext::Transfer<signed char> ; pop ecx ; ret 4
// Virtual persist hook (slot 1 of this class's vtable): transfers the single byte member at
// +0x30 through CPersistContext::Transfer<signed char>(name, value, default) @ 0x004045C0,
// with a zero default built in a one-byte stack temporary. The member name is taken
// from the persisted key. One of 44 bodies of this shape.
struct CPersistContext {
    void Transfer(const char* name, signed char& value, const signed char& defaultValue);   // Transfer<signed char> @ 0x004045C0
};

struct CPersistable_00eb69d0 {
    unsigned char _pad[0x2c];        // after the vptr; members not touched here
    signed char m_bHasBeenSpokenTo;                // +0x30
    virtual void v00();
    virtual void PersistTransfer_HasBeenSpokenTo_00eb69d0(CPersistContext* context);
};

void CPersistable_00eb69d0::PersistTransfer_HasBeenSpokenTo_00eb69d0(CPersistContext* context)
{
    signed char defaultValue = 0;
    context->Transfer("HasBeenSpokenTo", m_bHasBeenSpokenTo, defaultValue);
}
