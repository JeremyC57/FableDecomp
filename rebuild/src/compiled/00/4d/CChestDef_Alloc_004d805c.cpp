#pragma optimize("s",on)
// CChestDef::Alloc() -- factory: new CChestDef with the default constructor @ 0x004D3826 out of line
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D805C, oracle length 22 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push 0x3c ; call operator new ; test eax,eax ; pop ecx ; je N ; mov ecx,eax ; jmp CChestDef::CChestDef ; N: xor eax,eax ; ret
// A no-argument factory: allocates sizeof(CChestDef) = 0x3c bytes (operator new @ 0x00BFEA1A,
// landed-trusted), null-checks the block and tail-jumps into the out-of-line default constructor
// (which returns `this`). The class is named from the constructor's manifest label when that label
// is unique. One of 12 bodies of this shape.
struct CChestDef {
    unsigned char _body[0x3c];       // sizeof(CChestDef) = 0x3c
    CChestDef();                               // 0x004D3826
};

CChestDef* CChestDef_Alloc()
{
    return new CChestDef;
}