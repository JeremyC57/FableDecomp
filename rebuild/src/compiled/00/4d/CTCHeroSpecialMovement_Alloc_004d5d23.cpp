#pragma optimize("s",on)
// CTCHeroSpecialMovement::Alloc(CThing&) -- factory: new CTCHeroSpecialMovement(owner)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D5D23, oracle length 29 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0x1c ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCHeroSpecialMovement::CTCHeroSpecialMovement ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// A __fastcall factory: allocates sizeof(CTCHeroSpecialMovement) = 0x1c bytes (operator new @ 0x00BFEA1A,
// landed-trusted), null-checks the block and runs the constructor @ 0x007E08E0 on it with the
// argument, returning the new object (or null). One of 338 bodies of this shape; the class
// is taken from the constructor's manifest label. sizeof is modelled by an opaque body.

class CTCHeroSpecialMovement { public:
    unsigned char _body[0x1c];       // sizeof(CTCHeroSpecialMovement) = 0x1c
    CTCHeroSpecialMovement(void* owner);                   // 0x007E08E0
};

CTCHeroSpecialMovement* __fastcall CTCHeroSpecialMovement_Alloc(void* owner)
{
    return new CTCHeroSpecialMovement(owner);
}