#pragma optimize("s",on)
// CVillagePeopleDef::Alloc() -- factory: new CVillagePeopleDef with the default constructor @ 0x004525B3 out of line
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00453AE0, oracle length 25 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push 0xc4 ; call operator new ; test eax,eax ; pop ecx ; je N ; mov ecx,eax ; jmp CVillagePeopleDef::CVillagePeopleDef ; N: xor eax,eax ; ret
// A no-argument factory: allocates sizeof(CVillagePeopleDef) = 0xc4 bytes (operator new @ 0x00BFEA1A,
// landed-trusted), null-checks the block and tail-jumps into the out-of-line default constructor
// (which returns `this`). The class is named from the constructor's manifest label when that label
// is unique. One of 12 bodies of this shape.
class CVillagePeopleDef { public:
    unsigned char _body[0xc4];       // sizeof(CVillagePeopleDef) = 0xc4
    CVillagePeopleDef();                               // 0x004525B3
};

CVillagePeopleDef* CVillagePeopleDef_Alloc()
{
    return new CVillagePeopleDef;
}