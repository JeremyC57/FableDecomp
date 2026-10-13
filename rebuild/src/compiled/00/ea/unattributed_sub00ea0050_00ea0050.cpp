// CClass_00ea0050::sub_00ea0050() -- sets the byte flag at +0x1d
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EA0050, oracle length 5 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov byte ptr [ecx+0x1d],1 ; ret
// Virtual setter: writes 1 into the one-byte member at +0x1d and returns. The class comes
// from the vtable-slot label of this address; the flag's name is descriptive. One of 8 bodies.
struct CClass_00ea0050 {
    virtual void v00();
    virtual void sub_00ea0050();
    unsigned char _pad[0x19];
    bool m_flag;                           // +0x1d
};

void CClass_00ea0050::sub_00ea0050()
{
    m_flag = true;
}