#pragma optimize("s",on)
// NThingShot::CTCShotDefinitionFireballSpell::Alloc(CThing&) -- factory: new CTCShotDefinitionFireballSpell(owner)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DEB83, oracle length 32 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0xa4 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCShotDefinitionFireballSpell::CTCShotDefinitionFireballSpell ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// A __fastcall factory: allocates sizeof(CTCShotDefinitionFireballSpell) = 0xa4 bytes (operator new @ 0x00BFEA1A,
// landed-trusted), null-checks the block and runs the constructor @ 0x004DCAE9 on it with the
// argument, returning the new object (or null). One of 338 bodies of this shape; the class
// is taken from the constructor's manifest label. sizeof is modelled by an opaque body.

struct CTCShotDefinitionFireballSpell {
    unsigned char _body[0xa4];       // sizeof(CTCShotDefinitionFireballSpell) = 0xa4
    CTCShotDefinitionFireballSpell(void* owner);                   // 0x004DCAE9
};

CTCShotDefinitionFireballSpell* __fastcall CTCShotDefinitionFireballSpell_Alloc(void* owner)
{
    return new CTCShotDefinitionFireballSpell(owner);
}