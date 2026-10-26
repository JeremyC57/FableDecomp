#pragma optimize("s",on)
// CTCOnHeroPush::Alloc(CThing&) -- factory: new CTCOnHeroPush(owner)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D8CD7, oracle length 29 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0x20 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCOnHeroPush::CTCOnHeroPush ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// A __fastcall factory: allocates sizeof(CTCOnHeroPush) = 0x20 bytes (operator new @ 0x00BFEA1A,
// landed-trusted), null-checks the block and runs the constructor @ 0x004D5341 on it with the
// argument, returning the new object (or null). One of 338 bodies of this shape; the class
// is taken from the constructor's manifest label. sizeof is modelled by an opaque body.

class CTCOnHeroPush { public:
    unsigned char _body[0x20];       // sizeof(CTCOnHeroPush) = 0x20
    CTCOnHeroPush(void* owner);                   // 0x004D5341
};

CTCOnHeroPush* __fastcall CTCOnHeroPush_Alloc(void* owner)
{
    return new CTCOnHeroPush(owner);
}