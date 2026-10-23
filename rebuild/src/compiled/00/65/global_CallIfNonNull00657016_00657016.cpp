// CallIfNonNull_00657016(CTarget*) -- `__fastcall`: calls the target's method @ 0x00656EAD with 1 when the pointer is non-null
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00657016, oracle length 12 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  test ecx, ecx ; je 0x657021 ; push 1 ; call 0x656ead ; ret 
// Tiny-body shape `CallIfNonNull` synthesised from the instruction pattern; the manifest label is
// propagated where it is a plain name, the other names are descriptive.
struct CTarget {
    void Method_00656ead(long kind);                // 0x00656EAD
};

void __fastcall CallIfNonNull_00657016(CTarget* target)
{
    if (target)
        target->Method_00656ead(1);
}