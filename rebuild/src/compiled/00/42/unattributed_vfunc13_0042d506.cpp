// CClass_0042d506::vfunc_13() -- returns the 16-bit member at +0xc0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0042D506, oracle length 8 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  mov ax, word ptr [ecx + 0xc0] ; ret 
// Tiny-body shape `GetWord` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
struct CClass_0042d506 {
    unsigned char _pad[0xc0];
    short m_value;                         // +0xc0
    short vfunc_13();
};

short CClass_0042d506::vfunc_13()
{
    return m_value;
}