// CClass_00c3cfb0::HasIndex_00c3cfb0() -- non-zero (as a byte) when the index at +0xc is not -1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C3CFB0, oracle length 8 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  cmp dword ptr [ecx + 0xc], -1 ; setne al ; ret 
// Tiny-body shape `HasIndex` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
struct CClass_00c3cfb0 {
    unsigned char _pad[0xc];
    long m_index;                          // +0xc
    unsigned char HasIndex_00c3cfb0();                  // a byte result: a `bool` return would zero eax first
};

unsigned char CClass_00c3cfb0::HasIndex_00c3cfb0()
{
    return m_index != -1;
}