// CTCPhysicsStandard::vfunc_57() -- the 1-bit bitfield at bit 7 of the flag byte at +0x84
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00724C90, oracle length 10 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  mov al, byte ptr [ecx + 0x84] ; shr al, 7 ; ret 
// Tiny-body shape `GetHighBits` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
struct CTCPhysicsStandard {
    unsigned char _pad[0x84];
    unsigned char m_low : 7;              // bits 0..6 of the byte at +0x84
    unsigned char m_field : 1;             // bits 7..7
    bool vfunc_57();
};

bool CTCPhysicsStandard::vfunc_57()
{
    return m_field;
}