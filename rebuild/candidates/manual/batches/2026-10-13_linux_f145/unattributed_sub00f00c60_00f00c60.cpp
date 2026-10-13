// CClass_00f00c60::sub_00f00c60() -- sets the byte flag at +0x1c
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F00C60, oracle length 8 bytes.
// Grade: candidate -- still DIFFER(5v8) in the Linux lane (Wine + DDK 13.10.4035 cl.exe); generated for a
//        repeated body shape, see the batch README for the open question.
//
// Retail bytes:  mov byte ptr [ecx+0x1c],1 ; ret
// Virtual setter: writes 1 into the one-byte member at +0x1c and returns. The class comes
// from the vtable-slot label of this address; the flag's name is descriptive. One of 9 bodies.
struct CClass_00f00c60 {
    virtual void v00();
    virtual void sub_00f00c60();
    unsigned char _pad[0x18];
    bool m_flag;                           // +0x1c
};

void CClass_00f00c60::sub_00f00c60()
{
    m_flag = true;
}
