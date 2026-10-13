// CClass_009d8630::vfunc_0() -- sets the byte flag at +0x4
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009D8630, oracle length 5 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov byte ptr [ecx+0x4],1 ; ret
// Virtual setter: writes 1 into the one-byte member at +0x4 and returns. The class comes
// from the vtable-slot label of this address; the flag's name is descriptive. One of 8 bodies.
struct CClass_009d8630 {
    virtual void v00();
    virtual void vfunc_0();
    bool m_flag;                           // +0x4
};

void CClass_009d8630::vfunc_0()
{
    m_flag = true;
}