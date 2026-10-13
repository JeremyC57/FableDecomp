// Create_CPSCDecalRenderer() -- factory: new CPSCDecalRenderer with the default constructor @ 0x00B1DFD0 out of line
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AF8100, oracle length 24 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push 0x64 ; call operator new ; test eax,eax ; pop ecx ; je N ; mov ecx,eax ; jmp CPSCDecalRenderer::CPSCDecalRenderer ; N: xor eax,eax ; ret
// A no-argument factory: allocates sizeof(CPSCDecalRenderer) = 0x64 bytes (operator new @ 0x00BFEA1A,
// landed-trusted), null-checks the block and tail-jumps into the out-of-line default constructor
// (which returns `this`). The class is named from the constructor's manifest label when that label
// is unique. One of 11 bodies of this shape.
struct CPSCDecalRenderer {
    unsigned char _body[0x64];       // sizeof(CPSCDecalRenderer) = 0x64
    CPSCDecalRenderer();                               // 0x00B1DFD0
};

CPSCDecalRenderer* Create_CPSCDecalRenderer()
{
    return new CPSCDecalRenderer;
}
