#pragma optimize("s",on)
// CMultiStaticMeshDef::Alloc() -- factory: new CMultiStaticMeshDef with the default constructor @ 0x004E1516 out of line
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E31FA, oracle length 22 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push 0x34 ; call operator new ; test eax,eax ; pop ecx ; je N ; mov ecx,eax ; jmp CMultiStaticMeshDef::CMultiStaticMeshDef ; N: xor eax,eax ; ret
// A no-argument factory: allocates sizeof(CMultiStaticMeshDef) = 0x34 bytes (operator new @ 0x00BFEA1A,
// landed-trusted), null-checks the block and tail-jumps into the out-of-line default constructor
// (which returns `this`). The class is named from the constructor's manifest label when that label
// is unique. One of 12 bodies of this shape.
class CMultiStaticMeshDef { public:
    unsigned char _body[0x34];       // sizeof(CMultiStaticMeshDef) = 0x34
    CMultiStaticMeshDef();                               // 0x004E1516
};

CMultiStaticMeshDef* CMultiStaticMeshDef_Alloc()
{
    return new CMultiStaticMeshDef;
}