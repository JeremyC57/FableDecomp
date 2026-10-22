#pragma optimize("s",on)
// CTCSinglePersonOccupiable::Alloc(CThing&) -- factory: new CTCSinglePersonOccupiable(owner)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DB0AD, oracle length 29 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0x14 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCSinglePersonOccupiable::CTCSinglePersonOccupiable ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// A __fastcall factory: allocates sizeof(CTCSinglePersonOccupiable) = 0x14 bytes (operator new @ 0x00BFEA1A,
// landed-trusted), null-checks the block and runs the constructor @ 0x004D8896 on it with the
// argument, returning the new object (or null). One of 338 bodies of this shape; the class
// is taken from the constructor's manifest label. sizeof is modelled by an opaque body.
struct CInputProcessGameBaseInit;

struct CTCSinglePersonOccupiable {
    unsigned char _body[0x14];       // sizeof(CTCSinglePersonOccupiable) = 0x14
    CTCSinglePersonOccupiable(CInputProcessGameBaseInit* owner);                   // 0x004D8896
};

CTCSinglePersonOccupiable* __fastcall CTCSinglePersonOccupiable_Alloc(CInputProcessGameBaseInit* owner)
{
    return new CTCSinglePersonOccupiable(owner);
}