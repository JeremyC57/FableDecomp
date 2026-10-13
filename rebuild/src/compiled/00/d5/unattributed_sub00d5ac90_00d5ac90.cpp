// CClass_00d5ac90::sub_00d5ac90() -- sets the byte flag at +0x1c
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D5AC90, oracle length 5 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov byte ptr [ecx+0x1c],1 ; ret
// Virtual setter: writes 1 into the one-byte member at +0x1c and returns. The class comes
// from the vtable-slot label of this address; the flag's name is descriptive. One of 8 bodies.
struct CClass_00d5ac90 {
    virtual void v00();
    virtual void sub_00d5ac90();
    unsigned char _pad[0x18];
    bool m_flag;                           // +0x1c
};

void CClass_00d5ac90::sub_00d5ac90()
{
    m_flag = true;
}