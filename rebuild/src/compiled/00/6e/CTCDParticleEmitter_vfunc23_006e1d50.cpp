// CTCDParticleEmitter::vfunc_23() -- returns bit 2 of the flag byte at +0x44 (a one-bit bitfield, byte arithmetic)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E1D50, oracle length 9 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  mov al, byte ptr [ecx + 0x44] ; shr al, 2 ; and al, 1 ; ret 
// Tiny-body shape `GetFlagBitByte` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
struct CTCDParticleEmitter {
    unsigned char _pad[0x44];
    unsigned char m_bit0 : 1;              // bit 0
    unsigned char m_bit1 : 1;              // bit 1
    unsigned char m_flag : 1;               // bit 2 of the byte at +0x44: VC reads an unsigned char bitfield with byte shifts
    bool vfunc_23();
};

bool CTCDParticleEmitter::vfunc_23()
{
    return m_flag;
}