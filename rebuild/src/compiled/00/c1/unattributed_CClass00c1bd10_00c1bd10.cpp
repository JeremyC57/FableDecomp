// CClass_00c1bd10::CClass_00c1bd10() -- constructor: initialises through the out-of-line helper @ 0x00C1BC90
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C1BD10, oracle length 12 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  push esi ; mov esi, ecx ; call 0xc1bc90 ; mov eax, esi ; pop esi ; ret 
// Tiny-body shape `CtorInit`: a default constructor whose whole body is one out-of-line
// initialiser call; `this` is kept in esi for the return. The class comes from the manifest label.
struct CClass_00c1bd10 {
    unsigned char _body[4];
    void Initialise();                     // 0x00C1BC90
    CClass_00c1bd10();
};

CClass_00c1bd10::CClass_00c1bd10()
{
    Initialise();
}