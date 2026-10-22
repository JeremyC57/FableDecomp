#pragma optimize("s",on)
// CTCHeroStats::Alloc(CThing&) -- factory: new CTCHeroStats(owner)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D2FA9, oracle length 32 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0x1dc ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCHeroStats::CTCHeroStats ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// A __fastcall factory: allocates sizeof(CTCHeroStats) = 0x1dc bytes (operator new @ 0x00BFEA1A,
// landed-trusted), null-checks the block and runs the constructor @ 0x0057CDB5 on it with the
// argument, returning the new object (or null). One of 338 bodies of this shape; the class
// is taken from the constructor's manifest label. sizeof is modelled by an opaque body.
struct CThing;

struct CTCHeroStats {
    unsigned char _body[0x1dc];       // sizeof(CTCHeroStats) = 0x1dc
    CTCHeroStats(CThing* owner);                   // 0x0057CDB5
};

CTCHeroStats* __fastcall CTCHeroStats_Alloc(CThing* owner)
{
    return new CTCHeroStats(owner);
}