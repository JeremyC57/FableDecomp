// CClass_0054ff80::vfunc_93() -- sets the byte flag at +0x189
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0054FF80, oracle length 8 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov byte ptr [ecx+0x189],1 ; ret
// Virtual setter: writes 1 into the one-byte member at +0x189 and returns. The class comes
// from the vtable-slot label of this address; the flag's name is descriptive. One of 9 bodies.
struct CClass_0054ff80 {
    virtual void v00();
    virtual void vfunc_93();
    unsigned char _pad[0x185];
    bool m_flag;                           // +0x189
};

void CClass_0054ff80::vfunc_93()
{
    m_flag = true;
}