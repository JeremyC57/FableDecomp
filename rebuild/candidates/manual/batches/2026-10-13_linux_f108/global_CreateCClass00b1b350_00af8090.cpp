// Create_CClass_00b1b350() -- factory: new CClass_00b1b350 with the default constructor @ 0x00B1B350 out of line
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AF8090, oracle length 24 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push 0x40 ; call operator new ; test eax,eax ; pop ecx ; je N ; mov ecx,eax ; jmp CClass_00b1b350::CClass_00b1b350 ; N: xor eax,eax ; ret
// A no-argument factory: allocates sizeof(CClass_00b1b350) = 0x40 bytes (operator new @ 0x00BFEA1A,
// landed-trusted), null-checks the block and tail-jumps into the out-of-line default constructor
// (which returns `this`). The class is named from the constructor's manifest label when that label
// is unique. One of 11 bodies of this shape.
struct CClass_00b1b350 {
    unsigned char _body[0x40];       // sizeof(CClass_00b1b350) = 0x40
    CClass_00b1b350();                               // 0x00B1B350
};

CClass_00b1b350* Create_CClass_00b1b350()
{
    return new CClass_00b1b350;
}
