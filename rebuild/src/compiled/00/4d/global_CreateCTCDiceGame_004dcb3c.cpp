#pragma optimize("s",on)
// Create_CTCDiceGame(CThing*) -- factory: new CTCDiceGame(owner)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DCB3C, oracle length 32 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0xb4 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCDiceGame::CTCDiceGame ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// A __fastcall factory: allocates sizeof(CTCDiceGame) = 0xb4 bytes (operator new @ 0x00BFEA1A,
// landed-trusted), null-checks the block and runs the constructor @ 0x004DB15E on it with the
// argument, returning the new object (or null). One of 338 bodies of this shape; the class
// is taken from the constructor's manifest label. sizeof is modelled by an opaque body.
struct CThing;

struct CTCDiceGame {
    unsigned char _body[0xb4];       // sizeof(CTCDiceGame) = 0xb4
    CTCDiceGame(CThing* owner);                   // 0x004DB15E
};

CTCDiceGame* __fastcall Create_CTCDiceGame(CThing* owner)
{
    return new CTCDiceGame(owner);
}