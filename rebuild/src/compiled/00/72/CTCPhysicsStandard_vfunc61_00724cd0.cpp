// CTCPhysicsStandard::vfunc_61() -- returns bit 6 of the byte at +0x85
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00724CD0, oracle length 12 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov al,[ecx+0x85] ; shr al,6 ; and al,1 ; ret
// Virtual bitfield getter: bit 6 of the flag byte at +0x85, returned as a bool in al. The
// bitfield names are descriptive (vtable-slot label for the class). One of 11 bodies.
struct CTCPhysicsStandard {
    virtual void v00();
    virtual bool vfunc_61();
    unsigned char _pad[0x81];
    unsigned char m_bit0 : 1;
    unsigned char m_bit1 : 1;
    unsigned char m_bit2 : 1;
    unsigned char m_bit3 : 1;
    unsigned char m_bit4 : 1;
    unsigned char m_bit5 : 1;
    unsigned char m_bit6 : 1;
};

bool CTCPhysicsStandard::vfunc_61()
{
    return m_bit6;
}