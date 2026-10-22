#pragma optimize("s",on)
// CTCClock::Alloc(CThing&) -- factory: new CTCClock(owner)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D3422, oracle length 29 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0x14 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCClock::CTCClock ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// A __fastcall factory: allocates sizeof(CTCClock) = 0x14 bytes (operator new @ 0x00BFEA1A,
// landed-trusted), null-checks the block and runs the constructor @ 0x00732780 on it with the
// argument, returning the new object (or null). One of 338 bodies of this shape; the class
// is taken from the constructor's manifest label. sizeof is modelled by an opaque body.

struct CTCClock {
    unsigned char _body[0x14];       // sizeof(CTCClock) = 0x14
    CTCClock(long owner);                   // 0x00732780
};

CTCClock* __fastcall CTCClock_Alloc(long owner)
{
    return new CTCClock(owner);
}