// CClass_006b8410::Forward_006b8410() -- sibling jump to CObject_00881210::CScreenEffectPriorityStack_Rebuild on the embedded member at +0x90
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006B8410, oracle length 11 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  add ecx, 0x90 ; jmp 0x881210
// Tiny-body shape `Forward` synthesised from the instruction pattern (the class comes from the
// vtable-slot/manifest label when it is a plain class name, else the method is landed under a
// descriptive placeholder); member names are descriptive. The forwarded call keeps the signature, so VC7.1 turns it into a tail jump; the return type is modelled as long (a void or pointer result gives the same bytes).
struct CObject_00881210 {
    long CScreenEffectPriorityStack_Rebuild(); // 0x00881210
};

struct CClass_006b8410 {
    unsigned char _pad[0x90];
    CObject_00881210 m_member;
    long Forward_006b8410();
};

long CClass_006b8410::Forward_006b8410()
{
    return m_member.CScreenEffectPriorityStack_Rebuild();
}
