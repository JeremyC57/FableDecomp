#pragma optimize("s",on)
// CExplosionDef::Alloc() -- factory: new CExplosionDef with the default constructor @ 0x004E1341 out of line
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E3096, oracle length 22 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push 0x70 ; call operator new ; test eax,eax ; pop ecx ; je N ; mov ecx,eax ; jmp CExplosionDef::CExplosionDef ; N: xor eax,eax ; ret
// A no-argument factory: allocates sizeof(CExplosionDef) = 0x70 bytes (operator new @ 0x00BFEA1A,
// landed-trusted), null-checks the block and tail-jumps into the out-of-line default constructor
// (which returns `this`). The class is named from the constructor's manifest label when that label
// is unique. One of 12 bodies of this shape.
struct CExplosionDef {
    unsigned char _body[0x70];       // sizeof(CExplosionDef) = 0x70
    CExplosionDef();                               // 0x004E1341
};

CExplosionDef* CExplosionDef_Alloc()
{
    return new CExplosionDef;
}