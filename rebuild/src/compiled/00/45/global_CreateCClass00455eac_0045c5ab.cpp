#pragma optimize("s",on)
// Create_CClass_00455eac() -- factory: new CClass_00455eac with the default constructor @ 0x00455EAC out of line
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0045C5AB, oracle length 22 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push 0x64 ; call operator new ; test eax,eax ; pop ecx ; je N ; mov ecx,eax ; jmp CClass_00455eac::CClass_00455eac ; N: xor eax,eax ; ret
// A no-argument factory: allocates sizeof(CClass_00455eac) = 0x64 bytes (operator new @ 0x00BFEA1A,
// landed-trusted), null-checks the block and tail-jumps into the out-of-line default constructor
// (which returns `this`). The class is named from the constructor's manifest label when that label
// is unique. One of 12 bodies of this shape.
struct CClass_00455eac {
    unsigned char _body[0x64];       // sizeof(CClass_00455eac) = 0x64
    CClass_00455eac();                               // 0x00455EAC
};

CClass_00455eac* Create_CClass_00455eac()
{
    return new CClass_00455eac;
}