#pragma optimize("s",on)
// NThingShot::CTCShotDefinitionSpreadShotProjectile::Alloc(CThing&) -- factory: new CTCShotDefinitionSpreadShotProjectile(owner)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DEA93, oracle length 32 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0xa4 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCShotDefinitionSpreadShotProjectile::CTCShotDefinitionSpreadShotProjectile ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// A __fastcall factory: allocates sizeof(CTCShotDefinitionSpreadShotProjectile) = 0xa4 bytes (operator new @ 0x00BFEA1A,
// landed-trusted), null-checks the block and runs the constructor @ 0x004DCA3A on it with the
// argument, returning the new object (or null). One of 338 bodies of this shape; the class
// is taken from the constructor's manifest label. sizeof is modelled by an opaque body.

struct CTCShotDefinitionSpreadShotProjectile {
    unsigned char _body[0xa4];       // sizeof(CTCShotDefinitionSpreadShotProjectile) = 0xa4
    CTCShotDefinitionSpreadShotProjectile(void* owner);                   // 0x004DCA3A
};

CTCShotDefinitionSpreadShotProjectile* __fastcall CTCShotDefinitionSpreadShotProjectile_Alloc(void* owner)
{
    return new CTCShotDefinitionSpreadShotProjectile(owner);
}