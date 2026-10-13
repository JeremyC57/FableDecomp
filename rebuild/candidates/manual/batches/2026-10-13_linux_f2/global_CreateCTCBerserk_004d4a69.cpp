// Create_CTCBerserk(CThing*) -- factory: new CTCBerserk(owner)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D4A69, oracle length 29 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0x74 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCBerserk::CTCBerserk ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// A __fastcall factory: allocates sizeof(CTCBerserk) = 0x74 bytes (operator new @ 0x00BFEA1A,
// landed-trusted), null-checks the block and runs the constructor @ 0x0078C210 on it with the
// argument, returning the new object (or null). One of 338 bodies of this shape; the class
// is taken from the constructor's manifest label. sizeof is modelled by an opaque body.
struct CThing;

struct CTCBerserk {
    unsigned char _body[0x74];       // sizeof(CTCBerserk) = 0x74
    CTCBerserk(CThing* owner);                   // 0x0078C210
};

CTCBerserk* __fastcall Create_CTCBerserk(CThing* owner)
{
    return new CTCBerserk(owner);
}
