#pragma optimize("s",on)
// CObjectAugmentationsDef::Alloc() -- factory: new CObjectAugmentationsDef with the default constructor @ 0x004EBBA3 out of line
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004EC526, oracle length 25 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push 0x8c ; call operator new ; test eax,eax ; pop ecx ; je N ; mov ecx,eax ; jmp CObjectAugmentationsDef::CObjectAugmentationsDef ; N: xor eax,eax ; ret
// A no-argument factory: allocates sizeof(CObjectAugmentationsDef) = 0x8c bytes (operator new @ 0x00BFEA1A,
// landed-trusted), null-checks the block and tail-jumps into the out-of-line default constructor
// (which returns `this`). The class is named from the constructor's manifest label when that label
// is unique. One of 12 bodies of this shape.
class CObjectAugmentationsDef { public:
    unsigned char _body[0x8c];       // sizeof(CObjectAugmentationsDef) = 0x8c
    CObjectAugmentationsDef();                               // 0x004EBBA3
};

CObjectAugmentationsDef* CObjectAugmentationsDef_Alloc()
{
    return new CObjectAugmentationsDef;
}