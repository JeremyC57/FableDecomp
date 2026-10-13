#pragma optimize("s",on)
// Create_CTCInventoryAbilities(CThing*) -- factory: new CTCInventoryAbilities(owner)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D300F, oracle length 32 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0x1d8 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCInventoryAbilities::CTCInventoryAbilities ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// A __fastcall factory: allocates sizeof(CTCInventoryAbilities) = 0x1d8 bytes (operator new @ 0x00BFEA1A,
// landed-trusted), null-checks the block and runs the constructor @ 0x005F3BCF on it with the
// argument, returning the new object (or null). One of 338 bodies of this shape; the class
// is taken from the constructor's manifest label. sizeof is modelled by an opaque body.
struct CThing;

struct CTCInventoryAbilities {
    unsigned char _body[0x1d8];       // sizeof(CTCInventoryAbilities) = 0x1d8
    CTCInventoryAbilities(CThing* owner);                   // 0x005F3BCF
};

CTCInventoryAbilities* __fastcall Create_CTCInventoryAbilities(CThing* owner)
{
    return new CTCInventoryAbilities(owner);
}