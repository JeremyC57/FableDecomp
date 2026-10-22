#pragma optimize("s",on)
// NThingShot::CTCShotDefinitionTearInGround::Alloc(CThing&) -- factory: new CTCShotDefinitionTearInGround(owner)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D8E14, oracle length 32 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0x9c ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCShotDefinitionTearInGround::CTCShotDefinitionTearInGround ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// A __fastcall factory: allocates sizeof(CTCShotDefinitionTearInGround) = 0x9c bytes (operator new @ 0x00BFEA1A,
// landed-trusted), null-checks the block and runs the constructor @ 0x004D5500 on it with the
// argument, returning the new object (or null). One of 338 bodies of this shape; the class
// is taken from the constructor's manifest label. sizeof is modelled by an opaque body.

struct CTCShotDefinitionTearInGround {
    unsigned char _body[0x9c];       // sizeof(CTCShotDefinitionTearInGround) = 0x9c
    CTCShotDefinitionTearInGround(void* owner);                   // 0x004D5500
};

CTCShotDefinitionTearInGround* __fastcall CTCShotDefinitionTearInGround_Alloc(void* owner)
{
    return new CTCShotDefinitionTearInGround(owner);
}