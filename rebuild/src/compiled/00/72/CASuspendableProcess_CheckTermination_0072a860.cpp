// CASuspendableProcess::CheckTermination() -- non-zero (as a byte) when the byte at +0xc is non-zero
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0072A860, oracle length 9 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  mov al, byte ptr [ecx + 0xc] ; test al, al ; setne al ; ret 
// Tiny-body shape `IsSet` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
class CASuspendableProcess { public:
    unsigned char _pad[0xc];
    unsigned char m_value;                 // +0xc
    unsigned char CheckTermination();                   // a byte result: a `bool` return would zero eax first
};

unsigned char CASuspendableProcess::CheckTermination()
{
    return m_value != 0;
}