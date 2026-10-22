// CClass_00543a90::vfunc_93() -- sets the byte flag at +0x16c
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00543A90, oracle length 8 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov byte ptr [ecx+0x16c],1 ; ret
// Virtual setter: writes 1 into the one-byte member at +0x16c and returns. The class comes
// from the vtable-slot label of this address; the flag's name is descriptive. One of 9 bodies.
struct CClass_00543a90 {
    virtual void v00();
    virtual void vfunc_93();
    unsigned char _pad[0x168];
    bool m_flag;                           // +0x16c
};

void CClass_00543a90::vfunc_93()
{
    m_flag = true;
}