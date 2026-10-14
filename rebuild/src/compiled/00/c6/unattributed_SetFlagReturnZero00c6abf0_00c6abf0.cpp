// CClass_00c6abf0::SetFlagReturnZero_00c6abf0(long) -- stores 0 into the flag byte at +0x4 and returns 0; the argument is not read
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C6ABF0, oracle length 9 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  mov byte ptr [ecx + 4], 0 ; xor eax, eax ; ret 4
// Tiny-body shape `SetFlagReturnZero` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
struct CClass_00c6abf0 {
    unsigned char _pad[0x4];
    unsigned char m_flag;                  // +0x4
    long SetFlagReturnZero_00c6abf0(long unused);
};

long CClass_00c6abf0::SetFlagReturnZero_00c6abf0(long)
{
    m_flag = 0;
    return 0;
}