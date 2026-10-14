// CallIfNonNull_00656ffe(CTarget*) -- `__fastcall`: calls the target's method @ 0x00656E6F with 1 when the pointer is non-null
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00656FFE, oracle length 12 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  test ecx, ecx ; je 0x657009 ; push 1 ; call 0x656e6f ; ret 
// Tiny-body shape `CallIfNonNull` synthesised from the instruction pattern; the manifest label is
// propagated where it is a plain name, the other names are descriptive.
struct CTarget {
    void Method(long kind);                // 0x00656E6F
};

void __fastcall CallIfNonNull_00656ffe(CTarget* target)
{
    if (target)
        target->Method(1);
}