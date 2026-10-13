// CClass_009fd4d0::vfunc_9() -- sets the byte flag at +0x3a
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009FD4D0, oracle length 5 bytes.
// Grade: candidate -- still DIFFER(5v5) in the Linux lane (Wine + DDK 13.10.4035 cl.exe); generated for a
//        repeated body shape, see the batch README for the open question.
//
// Retail bytes:  mov byte ptr [ecx+0x3a],1 ; ret
// Virtual setter: writes 1 into the one-byte member at +0x3a and returns. The class comes
// from the vtable-slot label of this address; the flag's name is descriptive. One of 8 bodies.
struct CClass_009fd4d0 {
    virtual void v00();
    virtual void vfunc_9();
    unsigned char _pad[0x36];
    bool m_flag;                           // +0x3a
};

void CClass_009fd4d0::vfunc_9()
{
    m_flag = true;
}
