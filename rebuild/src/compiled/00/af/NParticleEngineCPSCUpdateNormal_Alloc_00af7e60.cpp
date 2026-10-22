// NParticleEngine::CPSCUpdateNormal::Alloc() -- factory: new CPSCUpdateNormal with the default constructor @ 0x00B0C9C0 out of line
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AF7E60, oracle length 27 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push 0xa0 ; call operator new ; test eax,eax ; pop ecx ; je N ; mov ecx,eax ; jmp CPSCUpdateNormal::CPSCUpdateNormal ; N: xor eax,eax ; ret
// A no-argument factory: allocates sizeof(CPSCUpdateNormal) = 0xa0 bytes (operator new @ 0x00BFEA1A,
// landed-trusted), null-checks the block and tail-jumps into the out-of-line default constructor
// (which returns `this`). The class is named from the constructor's manifest label when that label
// is unique. One of 11 bodies of this shape.
struct CPSCUpdateNormal {
    unsigned char _body[0xa0];       // sizeof(CPSCUpdateNormal) = 0xa0
    CPSCUpdateNormal();                               // 0x00B0C9C0
};

CPSCUpdateNormal* CPSCUpdateNormal_Alloc()
{
    return new CPSCUpdateNormal;
}