#pragma optimize("s",on)
// Create_CClass_004d5eca() -- factory: new CClass_004d5eca with the default constructor @ 0x004D5ECA out of line
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D92B0, oracle length 22 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push 0x30 ; call operator new ; test eax,eax ; pop ecx ; je N ; mov ecx,eax ; jmp CClass_004d5eca::CClass_004d5eca ; N: xor eax,eax ; ret
// A no-argument factory: allocates sizeof(CClass_004d5eca) = 0x30 bytes (operator new @ 0x00BFEA1A,
// landed-trusted), null-checks the block and tail-jumps into the out-of-line default constructor
// (which returns `this`). The class is named from the constructor's manifest label when that label
// is unique. One of 12 bodies of this shape.
struct CClass_004d5eca {
    unsigned char _body[0x30];       // sizeof(CClass_004d5eca) = 0x30
    CClass_004d5eca();                               // 0x004D5ECA
};

CClass_004d5eca* Create_CClass_004d5eca()
{
    return new CClass_004d5eca;
}