// Create_CModeCreatureIsProjectile(CThing*) -- factory: new CModeCreatureIsProjectile(owner)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0073A290, oracle length 34 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0x80 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CModeCreatureIsProjectile::CModeCreatureIsProjectile ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// A __fastcall factory: allocates sizeof(CModeCreatureIsProjectile) = 0x80 bytes (operator new @ 0x00BFEA1A,
// landed-trusted), null-checks the block and runs the constructor @ 0x008D82B0 on it with the
// argument, returning the new object (or null). One of 18 bodies of this shape; the class
// is taken from the constructor's manifest label. sizeof is modelled by an opaque body.
struct CThing;

struct CModeCreatureIsProjectile {
    unsigned char _body[0x80];       // sizeof(CModeCreatureIsProjectile) = 0x80
    CModeCreatureIsProjectile(CThing* owner);                   // 0x008D82B0
};

CModeCreatureIsProjectile* __fastcall Create_CModeCreatureIsProjectile(CThing* owner)
{
    return new CModeCreatureIsProjectile(owner);
}
