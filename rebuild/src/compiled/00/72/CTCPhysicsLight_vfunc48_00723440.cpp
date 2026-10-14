// CTCPhysicsLight::vfunc_48() -- returns the 1-bit bitfield at bit 0 of the flag byte at +0x40 (byte arithmetic)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00723440, oracle length 6 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  mov al, byte ptr [ecx + 0x40] ; and al, 1 ; ret 
// Tiny-body shape `GetLowBits` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
struct CTCPhysicsLight {
    unsigned char _pad[0x40];
    unsigned char m_field : 1;             // bits 0..0 of the byte at +0x40: VC masks an unsigned char bitfield in al
    bool vfunc_48();
};

bool CTCPhysicsLight::vfunc_48()
{
    return m_field;
}