#pragma optimize("s",on)
// CSoundThemeDef::Alloc() -- factory: new CSoundThemeDef with the default constructor @ 0x00455EAC out of line
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0045C5AB, oracle length 22 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push 0x64 ; call operator new ; test eax,eax ; pop ecx ; je N ; mov ecx,eax ; jmp CSoundThemeDef::CSoundThemeDef ; N: xor eax,eax ; ret
// A no-argument factory: allocates sizeof(CSoundThemeDef) = 0x64 bytes (operator new @ 0x00BFEA1A,
// landed-trusted), null-checks the block and tail-jumps into the out-of-line default constructor
// (which returns `this`). The class is named from the constructor's manifest label when that label
// is unique. One of 12 bodies of this shape.
struct CSoundThemeDef {
    unsigned char _body[0x64];       // sizeof(CSoundThemeDef) = 0x64
    CSoundThemeDef();                               // 0x00455EAC
};

CSoundThemeDef* CSoundThemeDef_Alloc()
{
    return new CSoundThemeDef;
}