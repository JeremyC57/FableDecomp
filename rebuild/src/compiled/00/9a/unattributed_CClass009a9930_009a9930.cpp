// CClass_009a9930::CClass_009a9930(const CAllocator&) -- constructor: initialises through the out-of-line helper @ 0x0099E4B0; the allocator argument is unused
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009A9930, oracle length 14 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  push esi ; mov esi, ecx ; call 0x99e4b0 ; mov eax, esi ; pop esi ; ret 4
// Tiny-body shape `CtorInitUnusedArg`: a constructor whose one argument (an empty allocator or a
// tag) is not read; `this` is kept in esi for the return while the out-of-line initialiser runs.
// The manifest label is propagated; the class is named from it when it is a plain class name.
struct CAllocator { };

struct CClass_009a9930 {
    unsigned char _body[4];
    void Initialise();                     // 0x0099E4B0
    CClass_009a9930(const CAllocator&);
};

CClass_009a9930::CClass_009a9930(const CAllocator&)
{
    Initialise();
}