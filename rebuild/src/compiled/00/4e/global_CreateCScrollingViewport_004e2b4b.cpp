#pragma optimize("s",on)
// Create_CScrollingViewport(long) -- factory: new CScrollingViewport(owner)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E2B4B, oracle length 29 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0x24 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CScrollingViewport::CScrollingViewport ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// A __fastcall factory: allocates sizeof(CScrollingViewport) = 0x24 bytes (operator new @ 0x00BFEA1A,
// landed-trusted), null-checks the block and runs the constructor @ 0x004E0C61 on it with the
// argument, returning the new object (or null). One of 338 bodies of this shape; the class
// is taken from the constructor's manifest label. sizeof is modelled by an opaque body.

struct CScrollingViewport {
    unsigned char _body[0x24];       // sizeof(CScrollingViewport) = 0x24
    CScrollingViewport(long owner);                   // 0x004E0C61
};

CScrollingViewport* __fastcall Create_CScrollingViewport(long owner)
{
    return new CScrollingViewport(owner);
}