#pragma optimize("s",on)
// CTCTavern::Alloc(CThing&) -- factory: new CTCTavern(owner)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D5215, oracle length 29 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0xc ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCTavern::CTCTavern ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// A __fastcall factory: allocates sizeof(CTCTavern) = 0xc bytes (operator new @ 0x00BFEA1A,
// landed-trusted), null-checks the block and runs the constructor @ 0x007BABC0 on it with the
// argument, returning the new object (or null). One of 338 bodies of this shape; the class
// is taken from the constructor's manifest label. sizeof is modelled by an opaque body.

struct CTCTavern {
    unsigned char _body[0xc];       // sizeof(CTCTavern) = 0xc
    CTCTavern(void* owner);                   // 0x007BABC0
};

CTCTavern* __fastcall CTCTavern_Alloc(void* owner)
{
    return new CTCTavern(owner);
}