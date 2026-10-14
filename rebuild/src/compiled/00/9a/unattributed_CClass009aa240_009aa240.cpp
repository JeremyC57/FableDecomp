// CClass_009aa240::CClass_009aa240() -- constructor: initialises through the out-of-line helper @ 0x0099E4B0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009AA240, oracle length 12 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  push esi ; mov esi, ecx ; call 0x99e4b0 ; mov eax, esi ; pop esi ; ret 
// Tiny-body shape `CtorInit`: a default constructor whose whole body is one out-of-line
// initialiser call; `this` is kept in esi for the return. The class comes from the manifest label.
struct CClass_009aa240 {
    unsigned char _body[4];
    void Initialise();                     // 0x0099E4B0
    CClass_009aa240();
};

CClass_009aa240::CClass_009aa240()
{
    Initialise();
}