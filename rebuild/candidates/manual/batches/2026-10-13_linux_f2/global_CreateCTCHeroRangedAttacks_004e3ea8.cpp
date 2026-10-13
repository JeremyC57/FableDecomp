// Create_CTCHeroRangedAttacks(CThing*) -- factory: new CTCHeroRangedAttacks(owner)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E3EA8, oracle length 29 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0x78 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCHeroRangedAttacks::CTCHeroRangedAttacks ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// A __fastcall factory: allocates sizeof(CTCHeroRangedAttacks) = 0x78 bytes (operator new @ 0x00BFEA1A,
// landed-trusted), null-checks the block and runs the constructor @ 0x004E30E7 on it with the
// argument, returning the new object (or null). One of 338 bodies of this shape; the class
// is taken from the constructor's manifest label. sizeof is modelled by an opaque body.
struct CThing;

struct CTCHeroRangedAttacks {
    unsigned char _body[0x78];       // sizeof(CTCHeroRangedAttacks) = 0x78
    CTCHeroRangedAttacks(CThing* owner);                   // 0x004E30E7
};

CTCHeroRangedAttacks* __fastcall Create_CTCHeroRangedAttacks(CThing* owner)
{
    return new CTCHeroRangedAttacks(owner);
}
