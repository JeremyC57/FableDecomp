#pragma optimize("s",on)
// Create_CClass_004e1516() -- factory: new CClass_004e1516 with the default constructor @ 0x004E1516 out of line
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E31FA, oracle length 22 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push 0x34 ; call operator new ; test eax,eax ; pop ecx ; je N ; mov ecx,eax ; jmp CClass_004e1516::CClass_004e1516 ; N: xor eax,eax ; ret
// A no-argument factory: allocates sizeof(CClass_004e1516) = 0x34 bytes (operator new @ 0x00BFEA1A,
// landed-trusted), null-checks the block and tail-jumps into the out-of-line default constructor
// (which returns `this`). The class is named from the constructor's manifest label when that label
// is unique. One of 12 bodies of this shape.
struct CClass_004e1516 {
    unsigned char _body[0x34];       // sizeof(CClass_004e1516) = 0x34
    CClass_004e1516();                               // 0x004E1516
};

CClass_004e1516* Create_CClass_004e1516()
{
    return new CClass_004e1516;
}