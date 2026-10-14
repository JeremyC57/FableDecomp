// CTCPhysicsStandard::vfunc_58() -- returns the 1-bit bitfield at bit 0 of the flag byte at +0x85 (byte arithmetic)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00724CA0, oracle length 9 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  mov al, byte ptr [ecx + 0x85] ; and al, 1 ; ret 
// Tiny-body shape `GetLowBits` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
struct CTCPhysicsStandard {
    unsigned char _pad[0x85];
    unsigned char m_field : 1;             // bits 0..0 of the byte at +0x85: VC masks an unsigned char bitfield in al
    bool vfunc_58();
};

bool CTCPhysicsStandard::vfunc_58()
{
    return m_field;
}