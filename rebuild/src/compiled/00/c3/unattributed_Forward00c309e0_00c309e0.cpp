// CClass_00c309e0::Forward_00c309e0() -- sibling jump to CObject_00c0ba40::Method_00c0ba40 on the embedded member at +0x4
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C309E0, oracle length 8 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  add ecx, 4 ; jmp 0xc0ba40
// Tiny-body shape `Forward` synthesised from the instruction pattern (the class comes from the
// vtable-slot/manifest label when it is a plain class name, else the method is landed under a
// descriptive placeholder); member names are descriptive. The forwarded call keeps the signature, so VC7.1 turns it into a tail jump; the return type is modelled as long (a void or pointer result gives the same bytes).
struct CObject_00c0ba40 {
    long Method_00c0ba40();     // 0x00C0BA40
};

struct CClass_00c309e0 {
    unsigned char _pad[0x4];
    CObject_00c0ba40 m_member;
    long Forward_00c309e0();
};

long CClass_00c309e0::Forward_00c309e0()
{
    return m_member.Method_00c0ba40();
}