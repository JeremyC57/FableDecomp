// CClass_005926c0::CClass_005926c0(const CAllocator&) -- constructor: initialises through the out-of-line helper @ 0x0099AED0; the allocator argument is unused
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005926C0, oracle length 14 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  push esi ; mov esi, ecx ; call 0x99aed0 ; mov eax, esi ; pop esi ; ret 4
// Tiny-body shape `CtorInitUnusedArg`: a constructor whose one argument (an empty allocator or a
// tag) is not read; `this` is kept in esi for the return while the out-of-line initialiser runs.
// The manifest label is propagated; the class is named from it when it is a plain class name.
struct CAllocator { };

struct CClass_005926c0 {
    unsigned char _body[4];
    void Initialise();                     // 0x0099AED0
    CClass_005926c0(const CAllocator&);
};

CClass_005926c0::CClass_005926c0(const CAllocator&)
{
    Initialise();
}