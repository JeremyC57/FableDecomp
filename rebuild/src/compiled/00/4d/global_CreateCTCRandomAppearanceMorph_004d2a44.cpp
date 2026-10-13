#pragma optimize("s",on)
// Create_CTCRandomAppearanceMorph(CThing*) -- factory: new CTCRandomAppearanceMorph(owner)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D2A44, oracle length 29 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0x2c ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCRandomAppearanceMorph::CTCRandomAppearanceMorph ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// A __fastcall factory: allocates sizeof(CTCRandomAppearanceMorph) = 0x2c bytes (operator new @ 0x00BFEA1A,
// landed-trusted), null-checks the block and runs the constructor @ 0x00728000 on it with the
// argument, returning the new object (or null). One of 338 bodies of this shape; the class
// is taken from the constructor's manifest label. sizeof is modelled by an opaque body.
struct CThing;

struct CTCRandomAppearanceMorph {
    unsigned char _body[0x2c];       // sizeof(CTCRandomAppearanceMorph) = 0x2c
    CTCRandomAppearanceMorph(CThing* owner);                   // 0x00728000
};

CTCRandomAppearanceMorph* __fastcall Create_CTCRandomAppearanceMorph(CThing* owner)
{
    return new CTCRandomAppearanceMorph(owner);
}