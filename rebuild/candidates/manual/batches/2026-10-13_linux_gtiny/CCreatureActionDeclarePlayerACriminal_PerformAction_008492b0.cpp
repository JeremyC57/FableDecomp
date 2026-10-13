// CCreatureAction_DeclarePlayerACriminal::PerformAction() -- sibling jump to CObject_00a01b50::Method_00a01b50 on the embedded member at +0x8
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008492B0, oracle length 8 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  add ecx, 8 ; jmp 0xa01b50
// Tiny-body shape `Forward` synthesised from the instruction pattern (the class comes from the
// vtable-slot/manifest label when it is a plain class name, else the method is landed under a
// descriptive placeholder); member names are descriptive. The forwarded call keeps the signature, so VC7.1 turns it into a tail jump; the return type is modelled as long (a void or pointer result gives the same bytes).
struct CObject_00a01b50 {
    long Method_00a01b50();     // 0x00A01B50
};

struct CCreatureAction_DeclarePlayerACriminal {
    unsigned char _pad[0x8];
    CObject_00a01b50 m_member;
    long PerformAction();
};

long CCreatureAction_DeclarePlayerACriminal::PerformAction()
{
    return m_member.Method_00a01b50();
}
