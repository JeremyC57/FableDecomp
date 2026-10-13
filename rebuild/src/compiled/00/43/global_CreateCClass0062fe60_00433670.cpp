// Create_CClass_0062fe60() -- factory: new CClass_0062fe60 with the default constructor @ 0x0062FE60 out of line
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00433670, oracle length 27 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push 0x228 ; call operator new ; test eax,eax ; pop ecx ; je N ; mov ecx,eax ; jmp CClass_0062fe60::CClass_0062fe60 ; N: xor eax,eax ; ret
// A no-argument factory: allocates sizeof(CClass_0062fe60) = 0x228 bytes (operator new @ 0x00BFEA1A,
// landed-trusted), null-checks the block and tail-jumps into the out-of-line default constructor
// (which returns `this`). The class is named from the constructor's manifest label when that label
// is unique. One of 11 bodies of this shape.
struct CClass_0062fe60 {
    unsigned char _body[0x228];       // sizeof(CClass_0062fe60) = 0x228
    CClass_0062fe60();                               // 0x0062FE60
};

CClass_0062fe60* Create_CClass_0062fe60()
{
    return new CClass_0062fe60;
}