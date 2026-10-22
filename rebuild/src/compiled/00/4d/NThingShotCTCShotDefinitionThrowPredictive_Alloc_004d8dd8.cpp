#pragma optimize("s",on)
// NThingShot::CTCShotDefinitionThrowPredictive::Alloc(CThing&) -- factory: new CTCShotDefinitionThrowPredictive(owner)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D8DD8, oracle length 32 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0x9c ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCShotDefinitionThrowPredictive::CTCShotDefinitionThrowPredictive ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// A __fastcall factory: allocates sizeof(CTCShotDefinitionThrowPredictive) = 0x9c bytes (operator new @ 0x00BFEA1A,
// landed-trusted), null-checks the block and runs the constructor @ 0x004D54B2 on it with the
// argument, returning the new object (or null). One of 338 bodies of this shape; the class
// is taken from the constructor's manifest label. sizeof is modelled by an opaque body.

struct CTCShotDefinitionThrowPredictive {
    unsigned char _body[0x9c];       // sizeof(CTCShotDefinitionThrowPredictive) = 0x9c
    CTCShotDefinitionThrowPredictive(void* owner);                   // 0x004D54B2
};

CTCShotDefinitionThrowPredictive* __fastcall CTCShotDefinitionThrowPredictive_Alloc(void* owner)
{
    return new CTCShotDefinitionThrowPredictive(owner);
}