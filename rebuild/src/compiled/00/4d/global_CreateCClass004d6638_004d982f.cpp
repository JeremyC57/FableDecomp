#pragma optimize("s",on)
// Create_CClass_004d6638() -- factory: new CClass_004d6638 with the default constructor @ 0x004D6638 out of line
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D982F, oracle length 22 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push 0x3c ; call operator new ; test eax,eax ; pop ecx ; je N ; mov ecx,eax ; jmp CClass_004d6638::CClass_004d6638 ; N: xor eax,eax ; ret
// A no-argument factory: allocates sizeof(CClass_004d6638) = 0x3c bytes (operator new @ 0x00BFEA1A,
// landed-trusted), null-checks the block and tail-jumps into the out-of-line default constructor
// (which returns `this`). The class is named from the constructor's manifest label when that label
// is unique. One of 12 bodies of this shape.
struct CClass_004d6638 {
    unsigned char _body[0x3c];       // sizeof(CClass_004d6638) = 0x3c
    CClass_004d6638();                               // 0x004D6638
};

CClass_004d6638* Create_CClass_004d6638()
{
    return new CClass_004d6638;
}