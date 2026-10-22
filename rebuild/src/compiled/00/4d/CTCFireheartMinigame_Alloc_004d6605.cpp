#pragma optimize("s",on)
// CTCFireheartMinigame::Alloc(CThing&) -- factory: new CTCFireheartMinigame(owner)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D6605, oracle length 32 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0x9c ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCFireheartMinigame::CTCFireheartMinigame ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// A __fastcall factory: allocates sizeof(CTCFireheartMinigame) = 0x9c bytes (operator new @ 0x00BFEA1A,
// landed-trusted), null-checks the block and runs the constructor @ 0x007E7640 on it with the
// argument, returning the new object (or null). One of 338 bodies of this shape; the class
// is taken from the constructor's manifest label. sizeof is modelled by an opaque body.
struct CThing;

struct CTCFireheartMinigame {
    unsigned char _body[0x9c];       // sizeof(CTCFireheartMinigame) = 0x9c
    CTCFireheartMinigame(CThing* owner);                   // 0x007E7640
};

CTCFireheartMinigame* __fastcall CTCFireheartMinigame_Alloc(CThing* owner)
{
    return new CTCFireheartMinigame(owner);
}