// Create_CTCTargetingPlayer(CThing*) -- factory: new CTCTargetingPlayer(owner)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E3783, oracle length 32 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0xf8 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCTargetingPlayer::CTCTargetingPlayer ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// A __fastcall factory: allocates sizeof(CTCTargetingPlayer) = 0xf8 bytes (operator new @ 0x00BFEA1A,
// landed-trusted), null-checks the block and runs the constructor @ 0x004E2250 on it with the
// argument, returning the new object (or null). One of 338 bodies of this shape; the class
// is taken from the constructor's manifest label. sizeof is modelled by an opaque body.
struct CThing;

struct CTCTargetingPlayer {
    unsigned char _body[0xf8];       // sizeof(CTCTargetingPlayer) = 0xf8
    CTCTargetingPlayer(CThing* owner);                   // 0x004E2250
};

CTCTargetingPlayer* __fastcall Create_CTCTargetingPlayer(CThing* owner)
{
    return new CTCTargetingPlayer(owner);
}
