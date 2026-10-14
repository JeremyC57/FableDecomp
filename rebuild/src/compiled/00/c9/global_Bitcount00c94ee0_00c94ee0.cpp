// Bitcount(unsigned long) -- `__fastcall`: number of shifts until the value is zero (bit length)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C94EE0, oracle length 14 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  xor eax, eax ; test ecx, ecx ; je 0xc94eed ; shr ecx, 1 ; inc eax ; test ecx, ecx ; jne 0xc94ee6 ; ret 
// Tiny-body shape `CountShifts`: a `__fastcall` bit-length helper -- counts how many times the
// register argument can be shifted right before it reaches zero (`while (v) { v >>= 1; ++n; }`).
// The manifest label is propagated.
unsigned long __fastcall Bitcount_00c94ee0(unsigned long value)
{
    unsigned long count = 0;
    while (value) {
        value >>= 1;
        ++count;
    }
    return count;
}