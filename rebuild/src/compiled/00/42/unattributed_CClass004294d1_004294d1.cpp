// CClass_004294d1::CClass_004294d1(const CAllocator&) -- constructor: initialises through the out-of-line helper @ 0x0052E5D0; the allocator argument is unused
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004294D1, oracle length 14 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  push esi ; mov esi, ecx ; call 0x52e5d0 ; mov eax, esi ; pop esi ; ret 4
// Tiny-body shape `CtorInitUnusedArg`: a constructor whose one argument (an empty allocator or a
// tag) is not read; `this` is kept in esi for the return while the out-of-line initialiser runs.
// The manifest label is propagated; the class is named from it when it is a plain class name.
struct CAllocator { };

struct CClass_004294d1 {
    unsigned char _body[4];
    void Initialise();                     // 0x0052E5D0
    CClass_004294d1(const CAllocator&);
};

CClass_004294d1::CClass_004294d1(const CAllocator&)
{
    Initialise();
}