#pragma optimize("s",on)
// Create_CTCGamePlayer(CThing*) -- factory: new CTCGamePlayer(owner)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DB1EA, oracle length 29 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0x18 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCGamePlayer::CTCGamePlayer ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// A __fastcall factory: allocates sizeof(CTCGamePlayer) = 0x18 bytes (operator new @ 0x00BFEA1A,
// landed-trusted), null-checks the block and runs the constructor @ 0x004D8FDD on it with the
// argument, returning the new object (or null). One of 338 bodies of this shape; the class
// is taken from the constructor's manifest label. sizeof is modelled by an opaque body.
struct CThing;

struct CTCGamePlayer {
    unsigned char _body[0x18];       // sizeof(CTCGamePlayer) = 0x18
    CTCGamePlayer(CThing* owner);                   // 0x004D8FDD
};

CTCGamePlayer* __fastcall Create_CTCGamePlayer(CThing* owner)
{
    return new CTCGamePlayer(owner);
}