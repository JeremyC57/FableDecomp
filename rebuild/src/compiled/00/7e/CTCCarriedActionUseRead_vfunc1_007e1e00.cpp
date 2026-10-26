// CTCCarriedActionUseRead::vfunc_1(CPersistContext* context, long unused) -- persists the "AlreadyRead" flag at +0x18
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E1E00, oracle length 33 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push ecx ; lea eax,[esp+3] ; push eax ; add ecx,0x18 ; push ecx ; mov ecx,[esp+10h] ; push offset "AlreadyRead" ; mov byte ptr [esp+0Fh],0 ; call CPersistContext::Transfer<signed char> ; pop ecx ; ret 8
// Virtual persist hook (slot 1 of this class's vtable): transfers the single byte member at
// +0x18 through CPersistContext::Transfer<signed char>(name, value, default) @ 0x004045C0,
// with a zero default built in a one-byte stack temporary. The second argument is not used by
// this body; its meaning is not established. The member name is taken
// from the persisted key. One of 44 bodies of this shape.
class CPersistContext { public:
    void Transfer(const char* name, signed char& value, const signed char& defaultValue);   // Transfer<signed char> @ 0x004045C0
};

struct CTCCarriedActionUseRead {
    unsigned char _pad[0x14];        // after the vptr; members not touched here
    signed char m_bAlreadyRead;                // +0x18
    virtual void v00();
    virtual void vfunc_1(CPersistContext* context, long unused);
};

void CTCCarriedActionUseRead::vfunc_1(CPersistContext* context, long unused)
{
    signed char defaultValue = 0;
    context->Transfer("AlreadyRead", m_bAlreadyRead, defaultValue);
}