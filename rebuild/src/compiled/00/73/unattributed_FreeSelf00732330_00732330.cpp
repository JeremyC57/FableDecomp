// CClass_00732330::FreeSelf_00732330() -- releases the object's own block through operator delete @ 0x00BFE9BC (no destructor runs)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00732330, oracle length 8 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  push ecx ; call 0xbfe9bc ; pop ecx ; ret 
// Tiny-body shape `FreeSelf` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. The one-slot cleanup is `pop ecx`, which the lander's size-pragma sweep reproduces.
struct CClass_00732330 {
    void FreeSelf_00732330();
};

void CClass_00732330::FreeSelf_00732330()
{
    ::operator delete(this);
}