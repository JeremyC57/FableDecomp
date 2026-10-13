#pragma optimize("s",on)
// Create_CTCSmokeGenerator(CThing*) -- factory: new CTCSmokeGenerator(owner)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D28EB, oracle length 29 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0x20 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCSmokeGenerator::CTCSmokeGenerator ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// A __fastcall factory: allocates sizeof(CTCSmokeGenerator) = 0x20 bytes (operator new @ 0x00BFEA1A,
// landed-trusted), null-checks the block and runs the constructor @ 0x00722540 on it with the
// argument, returning the new object (or null). One of 338 bodies of this shape; the class
// is taken from the constructor's manifest label. sizeof is modelled by an opaque body.
struct CThing;

struct CTCSmokeGenerator {
    unsigned char _body[0x20];       // sizeof(CTCSmokeGenerator) = 0x20
    CTCSmokeGenerator(CThing* owner);                   // 0x00722540
};

CTCSmokeGenerator* __fastcall Create_CTCSmokeGenerator(CThing* owner)
{
    return new CTCSmokeGenerator(owner);
}