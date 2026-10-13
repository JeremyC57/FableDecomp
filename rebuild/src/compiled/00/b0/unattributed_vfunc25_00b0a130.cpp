// CClass_00b0a130::vfunc_25() -- sets the byte flag at +0x97
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B0A130, oracle length 8 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov byte ptr [ecx+0x97],1 ; ret
// Virtual setter: writes 1 into the one-byte member at +0x97 and returns. The class comes
// from the vtable-slot label of this address; the flag's name is descriptive. One of 9 bodies.
struct CClass_00b0a130 {
    virtual void v00();
    virtual void vfunc_25();
    unsigned char _pad[0x93];
    bool m_flag;                           // +0x97
};

void CClass_00b0a130::vfunc_25()
{
    m_flag = true;
}