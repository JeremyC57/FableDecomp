#pragma optimize("s",on)
// CCarriedReadableDef::Alloc() -- factory: new CCarriedReadableDef with the default constructor @ 0x004D5ECA out of line
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D92B0, oracle length 22 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push 0x30 ; call operator new ; test eax,eax ; pop ecx ; je N ; mov ecx,eax ; jmp CCarriedReadableDef::CCarriedReadableDef ; N: xor eax,eax ; ret
// A no-argument factory: allocates sizeof(CCarriedReadableDef) = 0x30 bytes (operator new @ 0x00BFEA1A,
// landed-trusted), null-checks the block and tail-jumps into the out-of-line default constructor
// (which returns `this`). The class is named from the constructor's manifest label when that label
// is unique. One of 12 bodies of this shape.
struct CCarriedReadableDef {
    unsigned char _body[0x30];       // sizeof(CCarriedReadableDef) = 0x30
    CCarriedReadableDef();                               // 0x004D5ECA
};

CCarriedReadableDef* CCarriedReadableDef_Alloc()
{
    return new CCarriedReadableDef;
}