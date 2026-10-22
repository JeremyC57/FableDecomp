#pragma optimize("s",on)
// CTCDExperienceOrb::Alloc(CThing&) -- factory: new CTCDExperienceOrb(owner)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D434C, oracle length 29 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0x20 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCDExperienceOrb::CTCDExperienceOrb ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// A __fastcall factory: allocates sizeof(CTCDExperienceOrb) = 0x20 bytes (operator new @ 0x00BFEA1A,
// landed-trusted), null-checks the block and runs the constructor @ 0x00779CA0 on it with the
// argument, returning the new object (or null). One of 338 bodies of this shape; the class
// is taken from the constructor's manifest label. sizeof is modelled by an opaque body.
struct CThing;

struct CTCDExperienceOrb {
    unsigned char _body[0x20];       // sizeof(CTCDExperienceOrb) = 0x20
    CTCDExperienceOrb(CThing* owner);                   // 0x00779CA0
};

CTCDExperienceOrb* __fastcall CTCDExperienceOrb_Alloc(CThing* owner)
{
    return new CTCDExperienceOrb(owner);
}