#pragma optimize("s",on)
// CTCCombat::Alloc(CThing&) -- factory: new CTCCombat(owner)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D3513, oracle length 32 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0x98 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCCombat::CTCCombat ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// A __fastcall factory: allocates sizeof(CTCCombat) = 0x98 bytes (operator new @ 0x00BFEA1A,
// landed-trusted), null-checks the block and runs the constructor @ 0x00735540 on it with the
// argument, returning the new object (or null). One of 338 bodies of this shape; the class
// is taken from the constructor's manifest label. sizeof is modelled by an opaque body.
struct CThing;

struct CTCCombat {
    unsigned char _body[0x98];       // sizeof(CTCCombat) = 0x98
    CTCCombat(CThing* owner);                   // 0x00735540
};

CTCCombat* __fastcall CTCCombat_Alloc(CThing* owner)
{
    return new CTCCombat(owner);
}