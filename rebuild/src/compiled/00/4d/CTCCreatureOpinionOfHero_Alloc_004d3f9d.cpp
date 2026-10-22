#pragma optimize("s",on)
// CTCCreatureOpinionOfHero::Alloc(CThing&) -- factory: new CTCCreatureOpinionOfHero(owner)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D3F9D, oracle length 32 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0x350 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCCreatureOpinionOfHero::CTCCreatureOpinionOfHero ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// A __fastcall factory: allocates sizeof(CTCCreatureOpinionOfHero) = 0x350 bytes (operator new @ 0x00BFEA1A,
// landed-trusted), null-checks the block and runs the constructor @ 0x00684420 on it with the
// argument, returning the new object (or null). One of 338 bodies of this shape; the class
// is taken from the constructor's manifest label. sizeof is modelled by an opaque body.
struct CThing;

struct CTCCreatureOpinionOfHero {
    unsigned char _body[0x350];       // sizeof(CTCCreatureOpinionOfHero) = 0x350
    CTCCreatureOpinionOfHero(CThing* owner);                   // 0x00684420
};

CTCCreatureOpinionOfHero* __fastcall CTCCreatureOpinionOfHero_Alloc(CThing* owner)
{
    return new CTCCreatureOpinionOfHero(owner);
}