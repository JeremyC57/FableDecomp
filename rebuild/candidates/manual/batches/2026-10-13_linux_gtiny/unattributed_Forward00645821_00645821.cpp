// CClass_00645821::Forward_00645821() -- sibling jump to CObject_006445ba::ClearEffects on the embedded member at +0x28
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00645821, oracle length 8 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  add ecx, 0x28 ; jmp 0x6445ba
// Tiny-body shape `Forward` synthesised from the instruction pattern (the class comes from the
// vtable-slot/manifest label when it is a plain class name, else the method is landed under a
// descriptive placeholder); member names are descriptive. The forwarded call keeps the signature, so VC7.1 turns it into a tail jump; the return type is modelled as long (a void or pointer result gives the same bytes).
struct CObject_006445ba {
    long ClearEffects();        // 0x006445BA
};

struct CClass_00645821 {
    unsigned char _pad[0x28];
    CObject_006445ba m_member;
    long Forward_00645821();
};

long CClass_00645821::Forward_00645821()
{
    return m_member.ClearEffects();
}
