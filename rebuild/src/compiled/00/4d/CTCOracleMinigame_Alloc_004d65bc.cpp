#pragma optimize("s",on)
// CTCOracleMinigame::Alloc(CThing&) -- factory: new CTCOracleMinigame(owner)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D65BC, oracle length 32 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0xec ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCOracleMinigame::CTCOracleMinigame ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// A __fastcall factory: allocates sizeof(CTCOracleMinigame) = 0xec bytes (operator new @ 0x00BFEA1A,
// landed-trusted), null-checks the block and runs the constructor @ 0x007E5F00 on it with the
// argument, returning the new object (or null). One of 338 bodies of this shape; the class
// is taken from the constructor's manifest label. sizeof is modelled by an opaque body.
struct CThing;

struct CTCOracleMinigame {
    unsigned char _body[0xec];       // sizeof(CTCOracleMinigame) = 0xec
    CTCOracleMinigame(CThing* owner);                   // 0x007E5F00
};

CTCOracleMinigame* __fastcall CTCOracleMinigame_Alloc(CThing* owner)
{
    return new CTCOracleMinigame(owner);
}