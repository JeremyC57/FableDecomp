#pragma optimize("s",on)
// CTCEntityEvents::Alloc(CThing&) -- factory: new CTCEntityEvents(owner)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E48F6, oracle length 29 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0x10 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCEntityEvents::CTCEntityEvents ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// A __fastcall factory: allocates sizeof(CTCEntityEvents) = 0x10 bytes (operator new @ 0x00BFEA1A,
// landed-trusted), null-checks the block and runs the constructor @ 0x004E3CD7 on it with the
// argument, returning the new object (or null). One of 338 bodies of this shape; the class
// is taken from the constructor's manifest label. sizeof is modelled by an opaque body.
struct CTCEntityEvents;

struct CTCEntityEvents {
    unsigned char _body[0x10];       // sizeof(CTCEntityEvents) = 0x10
    CTCEntityEvents(CTCEntityEvents* owner);                   // 0x004E3CD7
};

CTCEntityEvents* __fastcall CTCEntityEvents_Alloc(CTCEntityEvents* owner)
{
    return new CTCEntityEvents(owner);
}