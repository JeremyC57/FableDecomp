// CClass_00787c1b::Forward_00787c1b() -- sibling jump to CFadeInFadeOutBase::IsOpen on the embedded member at +0x38
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00787C1B, oracle length 8 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  add ecx, 0x38 ; jmp 0x48da30
// Tiny-body shape `Forward` synthesised from the instruction pattern (the class comes from the
// vtable-slot/manifest label when it is a plain class name, else the method is landed under a
// descriptive placeholder); member names are descriptive. The forwarded call keeps the signature, so VC7.1 turns it into a tail jump; the return type is modelled as long (a void or pointer result gives the same bytes).
struct CFadeInFadeOutBase {
    long IsOpen();              // 0x0048DA30
};

struct CClass_00787c1b {
    unsigned char _pad[0x38];
    CFadeInFadeOutBase m_member;
    long Forward_00787c1b();
};

long CClass_00787c1b::Forward_00787c1b()
{
    return m_member.IsOpen();
}
