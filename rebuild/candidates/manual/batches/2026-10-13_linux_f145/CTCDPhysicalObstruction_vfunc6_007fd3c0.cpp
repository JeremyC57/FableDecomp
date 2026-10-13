// CTCDPhysicalObstruction::vfunc_6() -- sets the byte flag at +0x24
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007FD3C0, oracle length 8 bytes.
// Grade: candidate -- still DIFFER(5v8) in the Linux lane (Wine + DDK 13.10.4035 cl.exe); generated for a
//        repeated body shape, see the batch README for the open question.
//
// Retail bytes:  mov byte ptr [ecx+0x24],1 ; ret
// Virtual setter: writes 1 into the one-byte member at +0x24 and returns. The class comes
// from the vtable-slot label of this address; the flag's name is descriptive. One of 9 bodies.
struct CTCDPhysicalObstruction {
    virtual void v00();
    virtual void vfunc_6();
    unsigned char _pad[0x20];
    bool m_flag;                           // +0x24
};

void CTCDPhysicalObstruction::vfunc_6()
{
    m_flag = true;
}
