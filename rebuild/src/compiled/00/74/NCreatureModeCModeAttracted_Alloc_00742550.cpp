// NCreatureMode::CModeAttracted::Alloc(CThing&) -- factory: new CModeAttracted(owner)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00742550, oracle length 31 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0x74 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CModeAttracted::CModeAttracted ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// A __fastcall factory: allocates sizeof(CModeAttracted) = 0x74 bytes (operator new @ 0x00BFEA1A,
// landed-trusted), null-checks the block and runs the constructor @ 0x0073F170 on it with the
// argument, returning the new object (or null). One of 18 bodies of this shape; the class
// is taken from the constructor's manifest label. sizeof is modelled by an opaque body.
struct CThing;

struct CModeAttracted {
    unsigned char _body[0x74];       // sizeof(CModeAttracted) = 0x74
    CModeAttracted(CThing* owner);                   // 0x0073F170
};

CModeAttracted* __fastcall CModeAttracted_Alloc(CThing* owner)
{
    return new CModeAttracted(owner);
}