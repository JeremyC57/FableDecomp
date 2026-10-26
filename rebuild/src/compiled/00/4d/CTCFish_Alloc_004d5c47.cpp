#pragma optimize("s",on)
// CTCFish::Alloc(CThing&) -- factory: new CTCFish(owner)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D5C47, oracle length 29 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0x1c ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCFish::CTCFish ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// A __fastcall factory: allocates sizeof(CTCFish) = 0x1c bytes (operator new @ 0x00BFEA1A,
// landed-trusted), null-checks the block and runs the constructor @ 0x007DFB20 on it with the
// argument, returning the new object (or null). One of 338 bodies of this shape; the class
// is taken from the constructor's manifest label. sizeof is modelled by an opaque body.
class CThing;

class CTCFish { public:
    unsigned char _body[0x1c];       // sizeof(CTCFish) = 0x1c
    CTCFish(CThing* owner);                   // 0x007DFB20
};

CTCFish* __fastcall CTCFish_Alloc(CThing* owner)
{
    return new CTCFish(owner);
}