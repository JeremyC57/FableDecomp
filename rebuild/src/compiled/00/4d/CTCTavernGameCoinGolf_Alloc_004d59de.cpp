#pragma optimize("s",on)
// CTCTavernGameCoinGolf::Alloc(CThing&) -- factory: new CTCTavernGameCoinGolf(owner)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D59DE, oracle length 32 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0x180 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCTavernGameCoinGolf::CTCTavernGameCoinGolf ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// A __fastcall factory: allocates sizeof(CTCTavernGameCoinGolf) = 0x180 bytes (operator new @ 0x00BFEA1A,
// landed-trusted), null-checks the block and runs the constructor @ 0x007D49E0 on it with the
// argument, returning the new object (or null). One of 338 bodies of this shape; the class
// is taken from the constructor's manifest label. sizeof is modelled by an opaque body.
struct CThing;

struct CTCTavernGameCoinGolf {
    unsigned char _body[0x180];       // sizeof(CTCTavernGameCoinGolf) = 0x180
    CTCTavernGameCoinGolf(CThing* owner);                   // 0x007D49E0
};

CTCTavernGameCoinGolf* __fastcall CTCTavernGameCoinGolf_Alloc(CThing* owner)
{
    return new CTCTavernGameCoinGolf(owner);
}