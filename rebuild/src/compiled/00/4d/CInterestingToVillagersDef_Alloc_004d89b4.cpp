#pragma optimize("s",on)
// CInterestingToVillagersDef::Alloc() -- factory: new CInterestingToVillagersDef with the default constructor @ 0x004D4FCF out of line
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D89B4, oracle length 22 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push 0x2c ; call operator new ; test eax,eax ; pop ecx ; je N ; mov ecx,eax ; jmp CInterestingToVillagersDef::CInterestingToVillagersDef ; N: xor eax,eax ; ret
// A no-argument factory: allocates sizeof(CInterestingToVillagersDef) = 0x2c bytes (operator new @ 0x00BFEA1A,
// landed-trusted), null-checks the block and tail-jumps into the out-of-line default constructor
// (which returns `this`). The class is named from the constructor's manifest label when that label
// is unique. One of 12 bodies of this shape.
class CInterestingToVillagersDef { public:
    unsigned char _body[0x2c];       // sizeof(CInterestingToVillagersDef) = 0x2c
    CInterestingToVillagersDef();                               // 0x004D4FCF
};

CInterestingToVillagersDef* CInterestingToVillagersDef_Alloc()
{
    return new CInterestingToVillagersDef;
}