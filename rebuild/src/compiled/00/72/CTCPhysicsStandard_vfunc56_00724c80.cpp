// CTCPhysicsStandard::vfunc_56() -- returns bit 4 of the byte at +0x84
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00724C80, oracle length 12 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov al,[ecx+0x84] ; shr al,4 ; and al,1 ; ret
// Virtual bitfield getter: bit 4 of the flag byte at +0x84, returned as a bool in al. The
// bitfield names are descriptive (vtable-slot label for the class). One of 11 bodies.
struct CTCPhysicsStandard {
    virtual void v00();
    virtual bool vfunc_56();
    unsigned char _pad[0x80];
    unsigned char m_bit0 : 1;
    unsigned char m_bit1 : 1;
    unsigned char m_bit2 : 1;
    unsigned char m_bit3 : 1;
    unsigned char m_bit4 : 1;
};

bool CTCPhysicsStandard::vfunc_56()
{
    return m_bit4;
}