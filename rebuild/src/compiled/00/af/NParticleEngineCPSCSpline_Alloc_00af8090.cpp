// NParticleEngine::CPSCSpline::Alloc() -- factory: new CPSCSpline with the default constructor @ 0x00B1B350 out of line
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AF8090, oracle length 24 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push 0x40 ; call operator new ; test eax,eax ; pop ecx ; je N ; mov ecx,eax ; jmp CPSCSpline::CPSCSpline ; N: xor eax,eax ; ret
// A no-argument factory: allocates sizeof(CPSCSpline) = 0x40 bytes (operator new @ 0x00BFEA1A,
// landed-trusted), null-checks the block and tail-jumps into the out-of-line default constructor
// (which returns `this`). The class is named from the constructor's manifest label when that label
// is unique. One of 11 bodies of this shape.
struct CPSCSpline {
    unsigned char _body[0x40];       // sizeof(CPSCSpline) = 0x40
    CPSCSpline();                               // 0x00B1B350
};

CPSCSpline* CPSCSpline_Alloc()
{
    return new CPSCSpline;
}