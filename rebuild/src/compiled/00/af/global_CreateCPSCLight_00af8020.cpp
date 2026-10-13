// Create_CPSCLight() -- factory: new CPSCLight with the default constructor @ 0x00B1A640 out of line
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AF8020, oracle length 24 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push 0x6c ; call operator new ; test eax,eax ; pop ecx ; je N ; mov ecx,eax ; jmp CPSCLight::CPSCLight ; N: xor eax,eax ; ret
// A no-argument factory: allocates sizeof(CPSCLight) = 0x6c bytes (operator new @ 0x00BFEA1A,
// landed-trusted), null-checks the block and tail-jumps into the out-of-line default constructor
// (which returns `this`). The class is named from the constructor's manifest label when that label
// is unique. One of 11 bodies of this shape.
struct CPSCLight {
    unsigned char _body[0x6c];       // sizeof(CPSCLight) = 0x6c
    CPSCLight();                               // 0x00B1A640
};

CPSCLight* Create_CPSCLight()
{
    return new CPSCLight;
}